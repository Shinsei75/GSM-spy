#include <SoftwareSerial.h>
SoftwareSerial SIM800(7, 6); //Rx   Tx

String foo="";
void setup() {
  Serial.begin(19200);
  SIM800.begin(19200);
  Serial.println("Start");

  sendATCommand("AT", true);
  foo = sendATCommand("AT+CLIP=1", true); // включаем АОН
}

String sendATCommand(String cmd, bool waiting)
{
  String bar = ""; //переменная для хранения результата
  Serial.println(cmd);
  SIM800.println(cmd);

  if(waiting)
  {
    bar = waitResponse();
    if(bar.startsWith(cmd))
    {
      bar = bar.substring(bar.indexOf("\\r", cmd.length()) + 2);
    }
    Serial.println(bar);
  }
  return bar;
}

//Функция ожидания ответа и возврата полученного результата
String waitResponse()
{
  String baz = "";
  long timeout = millis() + 10000;

  while (!SIM800.available() && millis() < timeout) {};
  if (SIM800.available())
  {
    baz = SIM800.readString();
  }
  else
  {
    Serial.println("Timeout...");
  }
  return baz;
}
void loop() {
  if (SIM800.available())
  {
    foo = waitResponse();
    //убираем лишнии пробелы в начале и конце
    foo.trim();
    Serial.println(foo);
    //список телефонов с которых звонить, можно указать через запятую несколько штук
    String phone = "+7**********";
    // проверка при входящем вызове
    if (foo.startsWith("RING"))
    {
      int phoneindex = foo.indexOf("+CLIP: \"");
      // хранение определенного номера
      String inPhone = "";
      // если информация есть, то получаем номер
      if (phoneindex >= 0)
      {
        phoneindex += 8;
        inPhone = foo.substring(phoneindex, foo.indexOf("\"", phoneindex));
        Serial.println("Number: " + inPhone);
      }
      // проверка на присутствие номера в списке
      if (inPhone.length() >= 7 && phone.indexOf(inPhone) >= 0)
      {
        sendATCommand("ATA", true);
      }
      else
      {
        sendATCommand("ATH", true);
      }
    }
  }
  // ожидание команды и отправка её модему
  if (Serial.available())
  {
    SIM800.write(Serial.read());
  };
}
