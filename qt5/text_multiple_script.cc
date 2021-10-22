//
// Created by rick on 2021/10/21.
//
#include <QApplication>
#include <QLabel>

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  QFont font;
  font.setFamily("Segoe UI");
  font.setPointSize(40);
  app.setFont(font);

  QLabel label;
  label.setText(
      u8"When 🐉☺️❤️👮🏿👨‍👩‍👧‍👦🚵 in the Course of human "
      u8"events "
      "it "
      "मनीष منش"
      "Lettre à ma meilleure amie La célébration de Pâques \u00DC\u0041\u0308"
      "example@example.com "
      "吃葡萄不吐葡萄皮，落日覅"
      " https://example.com "
      "fi"
      "\u0066\u0303\u0069\u0323"
      "\u092B\u093F"
      "\u0061\u0308\u0303\u0323\u032D\u0308\u0303\u0323\u032D"
      "\u0E02\u0E36\u0E49"
      "סעיף א. כל בני אדם נולדו בני חורין ושווים בערכם"
      "station to which the Laws of Nature and of Nature's God entitle them, "
      "صباح الخير"
      "おはようございます");
  //  label.setFont(font);
  label.setWordWrap(true);
  label.show();
  return app.exec();
}