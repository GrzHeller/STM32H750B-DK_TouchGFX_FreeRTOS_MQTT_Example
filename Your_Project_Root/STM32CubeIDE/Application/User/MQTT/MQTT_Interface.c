#include <MQTT_Interface.h>
#include <string.h>

/* The idea is to demultiplex topic and create some reference to be used in data callbacks
 Example here uses a global variable, better would be to use a member in arg
 If RAM and CPU budget allows it, the easiest implementation might be to just take a copy of
 the topic string and use it in mqtt_incoming_data_cb
 */
static int inpub_id;
static void mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len) {
	printf("Incoming publish at topic %s with total length %u\n", topic, (unsigned int) tot_len);
	/* Decode topic string into a user defined reference */
	if (strcmp(topic, "print_payload") == 0) {
		inpub_id = 0;
	} else if (topic[0] == 'A') {
		/* All topics starting with 'A' might be handled at the same way */
		inpub_id = 1;
	} else {
		/* For all other topics */
		inpub_id = 2;
	}
}

static void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags) {
	printf("Incoming publish payload with length %d, flags %u\n", len, (unsigned int) flags);

	if (flags & MQTT_DATA_FLAG_LAST) {
		/* Last fragment of payload received (or whole part if payload fits receive buffer
		 See MQTT_VAR_HEADER_BUFFER_LEN)  */

		/* Call function or do action depending on reference, in this case inpub_id */

		// Here we parse the 0 and 1 sent over MQTT
		uint8_t *data8bit = arg;
		if (data[0] == '1') {
			*data8bit = '1';
		} else if (data[0] == '0') {
			*data8bit = '0';
		}

		if (inpub_id == 0) {
			/* Don't trust the publisher, check zero termination */
			if (data[len - 1] == 0) {
				printf("mqtt_incoming_data_cb: %s\n", (const char*) data);
			}
		} else if (inpub_id == 1) {
			/* Call an 'A' function... */
		} else {
			printf("mqtt_incoming_data_cb: Ignoring payload...\n");
		}
	} else {
		/* Handle fragmented payload, store in buffer, write to file or whatever */
	}
}

static void mqtt_sub_request_cb(void *arg, err_t result) {
	/* Just print the result code here for simplicity,
	 normal behaviour would be to take some action if subscribe fails like
	 notifying user, retry subscribe or disconnect from server */
	printf("Subscribe result: %d\n", result);
}

static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status) {
	err_t err;
	if (status == MQTT_CONNECT_ACCEPTED) {
		printf("mqtt_connection_cb: Successfully connected\n");

		/* Setup callback for incoming publish requests */
		mqtt_set_inpub_callback(client, mqtt_incoming_publish_cb, mqtt_incoming_data_cb, arg);

		/* Subscribe to a topic named "test/topic" with QoS level 0, call mqtt_sub_request_cb with result */
		err = mqtt_subscribe(client, "test/topic", 0, mqtt_sub_request_cb, arg);
		if (err != ERR_OK) {
			printf("mqtt_subscribe return: %d\n", err);
		}
	} else {
		printf("mqtt_connection_cb: Disconnected, reason: %d\n", status);

		/* Its more nice to be connected, so try to reconnect */
		//example_do_connect(client);
	}
}

/* Called when publish is complete either with sucess or failure */
static void mqtt_pub_request_cb(void *arg, err_t result) {
	if (result != ERR_OK) {
		printf("Publish result: %d\n", result);
	}
}

static void mqtt_resolved_cb(const char *name, const ip_addr_t *ipaddr, void *callback_arg) {
	//TODO this is a callback function on dns_gehostbyname
}

void interface_mqtt_connect(mqtt_client_t *client, const struct mqtt_connect_client_info_t *ci, void *arg) {
	err_t err;
	/* Initiate client and connect to server, if this fails immediately an error code is returned
	 otherwise mqtt_connection_cb will be called with connection result after attempting
	 to establish a connection with the server.
	 For now MQTT version 3.1.1 is always used */
	ip_addr_t mqttServerIP;

	/* CODE TO CHANGE BEGIN */
	// Input the local IP of the device hosting the broker on your network below
	err = dns_gethostbyname("192.168.1.3", &mqttServerIP, mqtt_resolved_cb, NULL);
	// Alternatively use the below line to connect to test.mosquitto.org
	//err = dns_gethostbyname("test.mosquitto.org", &mqttServerIP, mqtt_resolved_cb, NULL);
	/* CODE TO CHANGE END*/

	err = mqtt_client_connect(client, &mqttServerIP, MQTT_PORT, mqtt_connection_cb, arg, ci);

	/* For now just print the result code if something goes wrong */
	if (err != ERR_OK) {
		printf("mqtt_connect return %d\n", err);
	}
}

void interface_mqtt_publish(mqtt_client_t *client, void *arg) {
	const char *pub_payload = "PubSubHubLubJub";
	err_t err;
	u8_t qos = 0; /* 0 1 or 2, see MQTT specification */
	u8_t retain = 0; /* No don't retain such crappy payload... */
	err = mqtt_publish(client, "hello_world", pub_payload, strlen(pub_payload), qos, retain, mqtt_pub_request_cb, arg);
	if (err != ERR_OK) {
		printf("Publish err: %d\n", err);
	}
}
