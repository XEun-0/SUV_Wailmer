#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Create a main window
    QWidget window;
    window.setWindowTitle("Hello World Window");

    // Create a label
    QLabel *label = new QLabel("Hello, World!", &window);

    // Center the text
    label->setAlignment(Qt::AlignCenter);

    // Set layout
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    window.setLayout(layout);

    // Resize and show window
    window.resize(400, 300);
    window.show();

    // Execute the application
    return app.exec();
}
