/*************************************************************************
        > File Name: test_boost.cpp
        > Author:
        > Mail:
        > Created Time: 2019年04月08日 星期一 15时04分52秒
 ************************************************************************/

#include <iostream>
using namespace std;

#include <boost/signals2/signal.hpp>

class Button {
    typedef boost::signals2::signal<void (int x, int y)> OnClick;

    public:
      typedef OnClick::slot_type OnClickSlotType;
      // forward slots through Button interface to its private signal
      boost::signals2::connection doOnClick(const OnClickSlotType &slot);

      // simulate user clicking on GUI button at coordinate 52, 38
      void simulateClick();
    private:
      OnClick onClick;
};

boost::signals2::connection Button::doOnClick(const OnClickSlotType &slot) {
    return onClick.connect(slot);
}

void Button::simulateClick() {
    onClick(52, 38);
}

void printCoordinates(long x, long y) {
    std::cout << "(" << x << ", " << y << ")\n";
}

int main(int argc, char *argv[]) {
  Button button;
  button.doOnClick(&printCoordinates);
  button.simulateClick();

  return 0;
}
