#include <QtCore>
#include <cryptopp/aes.h>
#include <cryptopp/rsa.h>
#include <cryptopp/filters.h>
#include <cryptopp/modes.h>
#include <iomanip>
#include <iostream>
#include <string>

const QByteArray iv("0102030405060708");
const QByteArray presetKey("0CoJUm6Qyw8W8jud");
const QByteArray linuxapiKey("rFgB&h#%2?^eDg:Q");
const QByteArray
    base62("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
const QByteArray publicKey(
    "-----BEGIN PUBLIC "
    "KEY-----"
    "\nMIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDgtQn2JZ34ZC28NWYpAUd98iZ37BUrX/"
    "aKzmFbt7clFSs6sXqHauqKWqdtLkF2KexO40H1YTX8z2lSgBBOAxLsvaklV8k4cBFK9snQXE9/"
    "DDaFt6Rr7iVZMldczhC0JNgTz+"
    "SHXT6CBHuX3e9SdB1Ua44oncaTWz7OBGLbCiK45wIDAQAB\n-----END PUBLIC KEY-----");
const QByteArray eapiKey("e82ckenh8dichen8");

QByteArray aesEncrypt(const QByteArray &buffer, const QByteArray &key) {
  CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption encryptor(
      reinterpret_cast<const byte *>(key.data()),
      CryptoPP::AES::DEFAULT_KEYLENGTH,
      reinterpret_cast<const byte *>(iv.data()));

  std::string ciphertext;
  CryptoPP::StreamTransformationFilter stfEnc(
      encryptor, new CryptoPP::StringSink(ciphertext));
  stfEnc.Put(reinterpret_cast<const unsigned char *>(buffer.data()),
             buffer.length());
  stfEnc.MessageEnd();

  return QByteArray(ciphertext.c_str(), ciphertext.size());
}

QByteArray rsaEncrypt(const QByteArray& buffer,const QByteArray& key) {
  CryptoPP::StringSource ss(key.data(),true);
  CryptoPP::RSA::PublicKey pk;
  pk.Load(ss);

}

int main() {

  QByteArray plaintext(
      "Now is the time for all good men to come to the aide..");
  std::string ciphertext;
  std::string decryptedtext;

  qInfo() << QString("Plain Text (%1) bytes").arg(plaintext.size());

  // Create Cipher Text
  auto result = aesEncrypt(plaintext, presetKey);
  qDebug() << result.toBase64();

  return 0;
}