#include <QtCore>
#include <cryptopp/aes.h>
#include <cryptopp/rsa.h>
#include <cryptopp/filters.h>
#include <cryptopp/modes.h>
#include <cryptopp/base64.h>
#include <iomanip>
#include <iostream>
#include <string>

using namespace CryptoPP;

inline const QByteArray iv("0102030405060708");
inline const QByteArray presetKey("0CoJUm6Qyw8W8jud");
inline const QByteArray linuxapiKey("rFgB&h#%2?^eDg:Q");
inline const QByteArray
    base62("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
inline const QByteArray publicKey(
    "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDgtQn2JZ34ZC28NWYpAUd98iZ37BUrX/"
    "aKzmFbt7clFSs6sXqHauqKWqdtLkF2KexO40H1YTX8z2lSgBBOAxLsvaklV8k4cBFK9snQXE9/"
    "DDaFt6Rr7iVZMldczhC0JNgTz+"
    "SHXT6CBHuX3e9SdB1Ua44oncaTWz7OBGLbCiK45wIDAQAB");
inline const QByteArray eapiKey("e82ckenh8dichen8");

QByteArray aesEncrypt(const QByteArray &buffer, const QByteArray &key) {
  CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption encryptor(
      reinterpret_cast<const CryptoPP::byte *>(key.data()),
      CryptoPP::AES::DEFAULT_KEYLENGTH,
      reinterpret_cast<const CryptoPP::byte *>(iv.data()));

  std::string ciphertext;
  CryptoPP::StreamTransformationFilter stfEnc(
      encryptor, new CryptoPP::StringSink(ciphertext));
  stfEnc.Put(reinterpret_cast<const unsigned char *>(buffer.data()),
             buffer.length());
  stfEnc.MessageEnd();

  return QByteArray(ciphertext.c_str(), ciphertext.size());
}

QByteArray padForEncryption(const QByteArray& buffer,size_t targetLength) {
    auto paddingLength = targetLength - buffer.length();
    QByteArray result;
    result.append(buffer);
    result.append(paddingLength,0);

    std::reverse(result.begin(),result.end());
    return result;
}

QByteArray rsaEncrypt(const QByteArray& buffer, const QByteArray& key) {
    CryptoPP::StringSource ss(reinterpret_cast<const byte*>(key.data()),
                              key.length(), true,
                              new CryptoPP::Base64Decoder());
    try {
        CryptoPP::RSA::PublicKey pk;
        pk.Load(ss);
        QByteArray rawBuffer = padForEncryption(buffer,pk.GetModulus().ByteCount());
        qDebug() <<"buffer:" <<  rawBuffer.toHex();
        CryptoPP::Integer m(
            reinterpret_cast<const unsigned char*>(rawBuffer.data()),
            rawBuffer.length());
        CryptoPP::Integer c = pk.ApplyFunction(m);
        //        CryptoPP::RSAES_PKCS1v15_Encryptor encryptor(ss);
        //
        //        std::string ciphertext;
        //        CryptoPP::StringSource str(
        //            reinterpret_cast<const unsigned char*>(buffer.data()),
        //            buffer.length(), true,
        //            new CryptoPP::PK_EncryptorFilter(
        //                randomPool, encryptor, new
        //                CryptoPP::StringSink(ciphertext)));
        //
        QByteArray result;
        for (int i = c.ByteCount() - 1;i >=0;i--)
            result.append(c.GetByte(i));
        return result;
//        return QByteArray(ciphertext.c_str(), ciphertext.size());
    } catch (CryptoPP::Exception e) {
        std::cout << e.what() << std::endl;
        return QByteArray();
    }
}

int main() {

 QByteArray plaintext{"{\"radioId\":\"336355127\",\"limit\":30,\"offset\":0,"
                    "\"asc\":false,\"csrf_token\":\"\"}"};
  std::string ciphertext;
  std::string decryptedtext;

  qInfo() << QString("Plain Text (%1) bytes").arg(plaintext.size());

  // Create Cipher Text
  auto result = rsaEncrypt(presetKey, publicKey);
  qDebug() << result.toHex();

  return 0;
}