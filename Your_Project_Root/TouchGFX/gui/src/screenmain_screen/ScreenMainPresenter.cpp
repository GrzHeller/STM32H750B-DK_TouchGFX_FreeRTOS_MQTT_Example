#include <gui/screenmain_screen/ScreenMainView.hpp>
#include <gui/screenmain_screen/ScreenMainPresenter.hpp>

ScreenMainPresenter::ScreenMainPresenter(ScreenMainView &v) :
		view(v) {

}

void ScreenMainPresenter::activate() {

}

void ScreenMainPresenter::deactivate() {

}

void ScreenMainPresenter::setImage(uint8_t data) {
	view.updateImage(data);
}

void ScreenMainPresenter::setStatus(uint8_t status) {
	view.updateStatus(status);
}
