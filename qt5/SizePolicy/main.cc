//
// Created by xxzyjy on 2019-03-06.
//

#include <QApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QRadioButton>
#include <QWidget>

template <typename TYPE> void foo() {
    class Sub : public TYPE {};
}

#define BEGIN_LAYOUT(VAR, LAYOUT_VAR, TYPE, INIT_BLOCK)                        \
    TYPE* LAYOUT_VAR = nullptr;                                                \
    {                                                                          \
        class Sub##TYPE : public TYPE {                                        \
          public:                                                              \
            void init() { INIT_BLOCK }                                         \
        };                                                                     \
        Sub##TYPE* VAR##LAYOUT_VAR = new Sub##TYPE;                            \
        VAR##LAYOUT_VAR->init();                                               \
        VAR->setLayout(VAR##LAYOUT_VAR);                                       \
        LAYOUT_VAR = VAR##LAYOUT_VAR;

#define END_LAYOUT() }

struct GridLayoutParams;
template <typename T> struct GridLayoutParams_Property {
    GridLayoutParams* params;
    QString propName;
    GridLayoutParams_Property& operator=(T&& value);
};

struct GridLayoutParams {
    QVariantMap properties;
    GridLayoutParams_Property<int> row = {this, "row"};

    void addWidget(QGridLayout* layout, QWidget* widget) {
        int row                 = properties.value("row", 0).toInt();
        int column              = properties.value("column", 0).toInt();
        int rowSpan             = properties.value("rowSpan", 1).toInt();
        int columnSpan          = properties.value("columnSpan", 1).toInt();
        Qt::Alignment alignment = static_cast<Qt::Alignment>(
            properties.value("alignment", static_cast<int>(Qt::Alignment()))
                .toInt());

        layout->addWidget(widget, row, column, rowSpan, columnSpan, alignment);
    }
};

template <typename T>
GridLayoutParams_Property<T>&
GridLayoutParams_Property<T>::operator=(T&& value) {
    params->properties[propName] = QVariant(value);
    return *this;
}

struct QtPropertyValueSetter {
    virtual void setProperty(QWidget* widget) const = 0;
};
struct QtPropertyValue {
    QLatin1String propertyName;
    QVariant propertyValue;
    void setProperty(QWidget* widget) const {
        widget->setProperty(propertyName.latin1(), propertyValue);
    }
};

template <typename T> struct QtProperty {
    QLatin1String propertyName;
    QtPropertyValue operator=(T&& value) {
        return {propertyName, std::move(value)};
    }
};

#define NEW_WIDGET(VAR, TYPE, INIT)                                            \
    TYPE* VAR = nullptr;                                                       \
    {                                                                          \
        class Sub##TYPE : public TYPE {                                        \
          public:                                                              \
            void init() { INIT }                                               \
        };                                                                     \
        auto TEMP##VAR = new Sub##TYPE;                                        \
        TEMP##VAR->init();                                                     \
        VAR = TEMP##VAR;                                                       \
    }
#define NEW_WIDGET_PROP(VAR, TYPE, ...)                                        \
    TYPE* VAR = nullptr;                                                       \
    {                                                                          \
        VAR       = new TYPE;                                                  \
        auto init = {__VA_ARGS__};                                             \
        for (const auto& item : init) {                                        \
            item.setProperty(VAR);                                             \
        }                                                                      \
    }
QtProperty<QString> windowTitle = {QLatin1Literal("windowTitle")};
QtProperty<QRect> geometry      = {QLatin1Literal("geometry")};

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    // clang-format off
    NEW_WIDGET_PROP(MainWindow,QMainWindow,
        windowTitle = "Hello World",
        geometry = QRect(0,0,300,400)
    )
    NEW_LAYOUT(layout,QGridLayout)
    END_LAYOUT()
//    QMainWindow *MainWindow = new QMainWindow;
//    auto init = {windowTitle = "Hello World",geometry=QRect(0,0,300,400)};
//    for (const auto& item : init) {
//        item.setProperty(MainWindow);
//    }
    MainWindow->show();
//    NEW_WIDGET(MainWindow, QMainWindow, {
//        setGeometry(0, 0, 400, 413);
//        setWindowTitle("Style sheet");
//    })
//        NEW_WIDGET(centralWidget, QWidget, {})
//            BEGIN_LAYOUT(centralWidget, centralWidgetLayout, QVBoxLayout, {})
//                NEW_WIDGET(mainFrame,QFrame,{
//                    setFrameShape(QFrame::StyledPanel);
//                    setFrameShadow(QFrame::Raised);
//                })
//                BEGIN_LAYOUT(mainFrame,mainFrameLayout,QGridLayout,{})
//                    NEW_WIDGET(maleRadioButton,QRadioButton,{
//                        setToolTip("Check this if you are male");
//                        setText("&Male");
//                    })
//                    mainFrameLayout->addWidget(maleRadioButton,1,1);
//                END_LAYOUT()
//            END_LAYOUT()
    // clang-format on
    return app.exec();
}
