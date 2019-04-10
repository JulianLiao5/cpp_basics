/*************************************************************************
	> File Name: doc_view.cpp
	> Author: 
	> Mail: 
	> Created Time: 2019年04月09日 星期二 19时39分27秒
 ************************************************************************/

#include<iostream>
#include<string>
#include <algorithm>
// <utility>
using namespace std;

#include <boost/signals2/signal.hpp>


class Document {

  public:
    typedef boost::signals2::signal<void ()> signal_t;

  public:
    Document() {}
    /*Document(Document&& doc) : m_sig(std::move(doc.m_sig)) {
         this->m_text = doc.m_text;
    }*/

    ~Document() {}

    boost::signals2::connection connect(const signal_t::slot_type &subscriber) {
        return m_sig.connect(subscriber);
    }

    void append(const char* s) {
        printf("aaaa!\n");
        m_text += s;
        m_sig();
    }

    std::string getText() {
        return m_text;
    }
    // const std::string getText() const {
    //     return m_text;
    // }

  private:
    signal_t m_sig;
    std::string m_text;
};

class TextView {
  public:
    TextView(Document& doc) : m_document(doc) {
        m_connection = m_document.connect(boost::bind(&TextView::refresh, this));
    }

    ~TextView() {
        m_connection.disconnect();
    }

    void refresh() const {
        std::cout << "TextView: " << m_document.getText() << std::endl;
    }

  private:
    Document& m_document;
    boost::signals2::connection m_connection;
};

class HexView {
  public:
    HexView(Document& doc) : m_document(doc) {
        m_connection = m_document.connect(boost::bind(&HexView::refresh, this));
    }

    ~HexView() {
        m_connection.disconnect();
    }

    void refresh() const {
        const std::string& s = m_document.getText();

        std::cout << "HexView:";

        for (std::string::const_iterator it = s.begin(); it != s.end(); ++it) {
            std::cout << " " << std::hex << static_cast<int>(*it);
        }

        std::cout << "\n";
    }

  private:
    Document& m_document;
    boost::signals2::connection m_connection;
};

int main(int argc, char* argv[]) {
    Document doc;
    TextView v1(doc);
    HexView v2(doc);

    doc.append(argc == 2 ? argv[1] : "Hello world!");
    return 0;
}
