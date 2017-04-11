#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <SoftwareSerial.h>
#include <Wire.h>  // This library is already built in to the Arduino IDE
#include <LiquidCrystal_I2C.h> //This library you can add via Include Library > Manage Library > 
 
//const char* ssid = "TTNET_AirTies_Air5650_2T64";
//const char* password = "yrMnyEn4Ar";
const char* ssid = "ES_1847";
const char* password = "67200000";
const char* host = "api.pushetta.com";
String API("07687067502b881baaec0d86eed0e62cdc27640e");
String CHANNEL("Hasta ile ilgilenin");
String nemOrani = "";
String ortamSicakligi = "";
String vucutSicakligi = "";
String co = "";
String smoke = "";
String nabizDurumu = "";
String serialVeri="";
unsigned long eskizaman=0;
unsigned long yenizaman=0;
unsigned long yeniSeridenAlZaman = 0;
unsigned long eskiSeridenAlZaman = 0;

String webPageText()
{
  String temp = "<html lang=\"tr\"><head><meta charset=\"utf-8\"><meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><meta name=\"description\" content=\"\"><meta name=\"author\" content=\"\"><title>Anlık Hasta İzleme Sistemi</title><link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css\" integrity=\"sha384-BVYiiSIFeK1dGmJRAkycuHAHRg32OmUcww7on3RYdg4Va+PmSTsz/K68vbdEjh4u\" crossorigin=\"anonymous\"><link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap-theme.min.css\" integrity=\"sha384-rHyoN1iRsVXV4nD0JutlnGaslCJuC7uwjduW9SVrLvRYooPp2bWYgmgJQIXwl/Sp\" crossorigin=\"anonymous\"><script src=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js\" integrity=\"sha384-Tc5IQib027qvyjSMfHjOMaLkfuWVxZxUPnCJA7l2mCWNIpG9mGCD8wGNIcPD7Txa\" crossorigin=\"anonymous\"></script><script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.1.1/jquery.min.js\"></script><script src=\"http://malsup.github.io/jquery.blockUI.js\"></script><script>function cagir(linkText){$.ajax({url:linkText});}$(document).ajaxStop($.unblockUI);$(document).ready(function() {$('#btnac').click(function() {$.blockUI();});$('#btnkp').click(function() {$.blockUI({ message: '<h1><img src=\"busy.gif\" /> Just a moment...</h1>' });});});</script></head><body><div class=\"container theme-showcase\" role=\"main\"><div class=\"jumbotron\" align=\"center\"><h1>Anlık Hasta İzleme Sistemi</h1><p>Bu uygulama, ESP8266 modülü kullanarak responsive özelliği olan bir web sayfası üzerinden bir hastanın hayati verilerini izlemeye yarar.</p><p>Selçuk Üniversitesi Teknoloji Fakültesi 2016</p><p>İlkay Taşlı</p></div><div class=\"col-md-4\"><div class=\"jumbotron\" align=\"center\"><img border=\"0\" src=\"http://www.adabasini.com/resimler/2/serin-ve-nemli-hava-surecek-51839.jpg\" width=\"150\" height=\"100\"><h2>Nem Oranı</h2><p>"+nemOrani+"</p></div></div><div class=\"col-md-4\"><div class=\"jumbotron\" align=\"center\"><img border=\"0\" src=\"http://www.nukleonlab.com.tr/images/urunler/Civali-Termometreler-378.jpg\" width=\"150\" height=\"100\"><h2>Ortam Sıcaklığı</h2><p>"+ortamSicakligi+"</p></div></div><div class=\"col-md-4\"><div class=\"jumbotron\" align=\"center\"><img border=\"0\" src = \"http://www.beycan.net/img/5/12/termometre-cesitleri-ve-ozellikleri-300x232.jpg\" width=\"150\" height=\"100\" ><h2>Vücut Sıcaklığı</h2><p>"+ vucutSicakligi+"</p></div></div><div class=\"col-md-4\"><div class=\"jumbotron\" align=\"center\"><img border=\"0\" src = \"http://www.hvtd.org/yeni/wp-content/uploads/2012/08/co.gif\" width=\"150\" height=\"100\" ><h2>KarbonMonoksit</h2><p>"+co+"</p></div></div><div class=\"col-md-4\"><div class=\"jumbotron\" align=\"center\"><img border=\"0\" src = \"http://www.haberedavet.com/files/uploads/news/default/sigara-dumani-orucu--c1395e920315dc50fd6e.jpg\" width=\"150\" height=\"100\" ><h2>Duman</h2><p>"+smoke+"</p></div></div><div class=\"col-md-4\"><div class=\"jumbotron\" align=\"center\"><img border=\"0\" src = \"data:image/jpeg;base64,/9j/4AAQSkZJRgABAQAAAQABAAD/2wCEAAkGBxMSEhUSEhIWFRUVFxcVFRUVGBcVFRUXFRgaFxUVFRcYHSggGBolHRUVITEhJSkrLi4uFx8zODMtNygtLisBCgoKDg0OFxAPGisdHR0tLS0tLS0tLS0tLSsvNy8tKy0rLTcuKy0vLS0rKy0tLS0tLSswLSsrKystKzcwKzUvLf/AABEIALcBEwMBIgACEQEDEQH/xAAbAAACAwEBAQAAAAAAAAAAAAABAgADBAYFB//EAEoQAAECAgUIBwIMAwYHAAAAAAEAAgMRBBIhMVEFBkFhcbHB8AcTgZGh0eEi0hQjMlJUYmOTorLC8UOSoxUXNEKCsyQlU2Ryc6T/xAAZAQEAAwEBAAAAAAAAAAAAAAAAAgMEAQX/xAAwEQEAAgIAAwUGBgMBAAAAAAAAAQIDEQQSITEyM1FxEyIjQWHwBSSBobHhU5HRUv/aAAwDAQACEQMRAD8A+JPZadpUDbDzpSxLztKjbignVlMW2c4BVJzcO3ggnVlM9s1WmiXoC1nHcvdzezXi0xsZ0O6CwxHTwbfLvXgsv79y+udD0MfBo32jmwux7I093grsNItPVn4rLOOm4fJ3QyJg4+arqFaspMlFiD6yxqu0amYXUndYk7m3c6SoGIP0bOJQaopGcwzKLWm1I+8os07EEqJqtktvBVJ9Hb5IJUK2ZMyc+kRWQmfKeWtG02BYV2HRfBrZRgfVLXdjZOO5Tx15rRCvNaaUm0PGy/kSJQ47oET5TDIyu7DgvK6sruulVh+FQ36XwYLjtMJk/Ga4Jdy15bTCPD5PaY4ssDbEOrUFxSKtctc27ZxKWoVHXDnSlQO9lqjGcdyV95UZwO5dBLCmDbJa/MKsphd2+a4JUKZzdyrCL+AQOxtqWoUGXhBBYGm3nSEvVlRulIgJCiCiCyIBM26SoAJG1CJedp3oNuKCSGKYykq07rh2oJIYoulNVp4l6AtAnfivsXRS2rRoR+dTII7AHz/OvjcO/v3L6/mLEqUGjHGljwazzWrhO9Po8/8AEvCj1fM8vw6tIiA/OXmyGK97PiHVptIH2jvBxXPqnL35asHhV9IO4Cy3maDQMUH6NnEoNVa4zgMUWySvvKjNOxdEkMU0hK/mxVp9HeuCSGK7booA+Gh2EOIe6E88Fwy7nouHx8V2ECOf/niK7B34ZuLnWG3382zpVb7VFdjBZ4Fzf0+C+eyGK+h9JFsGhP8As3DujxuBC+cqXFR8SVf4fO8FVglI2oSGKAuKVZ21a6VnOKWQx8FHXDYgAgZwE1GAb9yLxadqZjdxXXNkLRj4IgDFNUTBlnb5o5zKaoxReBinqKPbdsCGyMAnegQMUzBaEhCOmErbUJDHwUbpSLjolRBRBZENps0lQG+xCILTtKjbjzpCAVtSYmy5JJM64dvBBKww3ovNtySSaILUBadWO5fT8kRqmTqKR9IinuZAlvXy9nA7ivpR9nJ1C1xYx8II4LXwnel534jG6RDnukplXKNJs/iv/O5ctWGG9dh0qN/5hGPznVv5reK42Soy9+Wvh/CqdztXMyg06kHaOdJUaFWuFzrTZvRa7VvSvvKjNK6JW1b007LubEkk2jnUglbUu76MB7VKOFGi+LJcVwUl33RpYymHCjP8ag4q3B34ZeM8KV+e5rUChu1xR3RJ8V87rDDevoWcprZKopwjRm/7R/UvnklLie/tDgfD15ScGw2IVtW9QXFQNWdsObhZvTNGrejVsHOlWNYpITbSVbbt/mnYzVzJWVLVZDZz2KUQpm6ipq3pqmrerurTBli7pDnZS3VvQe3UtPVoPYmnYuyBtt29UuGrettS1UPYo6W1uoxs34pK2rerS2/ZxCqLVGVsSUlRRRcSPEcZm3SVA4yPOkKRG2m68qBt6Ba5xTFxkOcEtTWExbYOcEC1zime4z/dCpsRe21BGOO9fR8qGWTsn63RPzN8l84Y3ivoecJlQMnD/wAz3xJfpWnh+m5YeMjfLH38mDpV/wAa44thu/mY08VxNc4ruOlITpEM4wYB/oM8lxFXYq8/fldws7xR+v8AIucbLeZoB5xRc27nSg1uxVNCOcZ3otecSg5tpuRa3YgWucU1Yy7fJLV2JqtmjmSBa5xXd9HzpQKcf+33xYI4rhKmsLu8x2yodPd9i0d8aD5K/h++ycb4UrspGtkeEfm0iJ4she6vn1c4r6A4TyLspB8WD3V8/DNi7xHbHo5wfSLR9TtcZfunYTigxtiuhw9neqYabTo7J2W71c2eKjWXXK5kNTiGa9hlarGNTiGrWQ1PTPa6kApqpV4hJurXdK+dkkle0rWYSR8NNOxdikqXgraWKh8NRmF1bMbp286VmcTiVuey/nSsz4exVzDVSzMVExCiiu2WJedp3oNuKd8pm+8oAC2/khcdVpjcEZDWmIEtOnggrTRL1JDWu4zfzF+E0Z0d0Sq81uqZI/GGG2u+3RJspTvJkp0pN+xVly1xxuzh2ea77O4yoeTh9Qn+vE8lw74VV5aZzBIXb59yFHyePsQe+NEKuxdK2Z+I65Mevqq6TvlUY40eB/tgcFwq73pKtZQnY0eH4V2/pXCSGtQ4jvynwfhQD9HOkoNVhA18lAAa1S1FdeVGadiYgTN6LQNaCpPo7+CgA1roszs3vhkQtLi2GwOe90p1WtEyZacJYkKVazadQhkvFKza3ZDml9AzLEsnU8/Uhj+o08FzudmQfgdIdCnWbYWuFzmkTa4aiC09q6PNSzJdOOqEPxT/AEq7DWa3mJZeJvF8UTHzTJ5nkSLqpEM97IvkuAC7/IBnkelDCLCPhFC4NoGtczdlfR3hu28fU8MWFaIYVcMCWlaIYGtVwtySuaLlewJGC5aIYCshjvJ2tV8NqjW2q+G1WxDLaxAxNUVzWp6ilpRN2MsVcRq2uYqYjVyYTrZic1ZntW9zVmiAKuWmlmJ4vWWI1b3gW86VliAKuYa6SxkIKwqKDRtQ8TJ28U7YDpH2T3HFXUOmdVFbEqtdVdWk4AgyNxBvGpdcOkR30SjfcwvdUqVrPenTmW+Ss+5Xbieof813cUxgOkPZd3HUuz/vGd9Eo33ML3UT0iu+iUb7mF7qnyY//Sr2uf8Ax/vDjaPRHucBVNpGgr6nTMr/AAKPQKOP4TGOiD60Q9Y4H/SWt7Fzo6R3aKJRp6PiYXurmMrZYiR4xjRDN5Naes28VOt6Y492dq74smeY541Efy9bPPJBg06IxrSW1jVIFhB+Se0EHtXrdJJlDoIwo7PFzzxW/M/PE0mNBo1IhQntcWwi9zGmJVMmD25VpgSlsCw9LLqsaDDMpsgwgRgagJHeSp25eS1ontVUtf2tKXjXLsOkK2i0B32IHdEihcIILjc09xX0DOmHXoWTtbXD+q/zXoZzZwtye6HR4dGgOlChFxfCY5xc6G1zySRO9xHYuZMcWtMzOodwZ7UpWlY3M7fNoNCiPc1rWGZ1ayu1h5qUSiMa7KEVwiOAPUsAL2g3V5kBmy04gKl3SPFbbDo8CG7Q9sKGHNOLSG2FcjTae+M8viOrOJnM61XvHTs6r9Zss6n3I/d2pzVoVLBFBpB63RCigNL9THAlpOoynoXF0rJ8SE5zHMIImDYdCSjUt0N9ZhkQV2LekiK4TiQIER0rXvhQ3OdLS5xbMnWU3jv29DlzYp933ocSID/mu7iu/wA3Z0PJkeORVfFc2E3QZD4x/wCWGP8AUsv94rvolG+5he6vIzkzri0xrWuaxjGTkxjWsaJ2k1WiUzjqXazTHuYnco3jLm1W1dR8+r3892fCaHRKU0TNQwnytM4Zs/A6H3JciNLckUuYlN8EeEU8F5GbeeMSjMMGpDiQy4OqxGNeAbpgOBkZEhd9nm5n9lGKxrWiOYLpNADawZFa+QF3tAmWtW1mtt3ieumbJF6cuKY6b6OYzRtyXTRgYTvxEfqXCwoTjcCdi7rMV06BT2/UYe6KzzWzIEZlDyaaT1UN73xag6xjXgBrS50g4WTrs7lDk561+/Jb7X2Vr6jczP8A1wsGivl8k9y63IGagdD6+kv6qCLKxEy4j/LDb/mPcBpIVjOkA/RaP9zC91eVlnOONSnAvMgBJrQAGtAuDWiwDUFyIx1672je2a/TXK6ZmT8mRPZZGisdoc9jS3tqumOwFeTljIMSjvkRWaRWa5vtNc03OaReF4UN8pLqMlZ1xYbBDc1kRoMwIjGvDSb6tYWT1KyLVt29Ga9L07J28tsF07itEOCcCvbbncfo9H+5h+6rmZ1H/oUf7mH7qnFa+bNa1vL7/wBvNodFEi94MhYBpccNi0l5lMwm1cJcb161Hzic9plBgTFsuph2jVYmpWdTolGNH6qGAXB02sDTPss8JqUVr5qZtPp+n9ucp0ACTm/JddPQReF58QL1ab7LGsN8y4jCdwXlxSq11WVwWWIFrcVliFVWaqMzheskQLY8rJEcq5baMxCiLlFFoZntMztKgaZFCJedpQbcVWvSoUxaZDt4JETcEEqHBM9pmkTRL0F1CjOhvD22EWgrVlvK0alxDFjOLnm8m8yAHALzmX9+5AqXNOtI8kc3Nrq+m09lbJ+Tzg+KO4tPFeL0q/4+IPmyb/KA3gvcoYr5Nof1Y0Vv4YR4rnelB88pUj/2RPzlbM3hxLyuFn48x5OVc02c6UAwqP0bOJStWF65nNMyi1pSuvKLNOxAKhTVTLnUq0+jnUglQr1qVl6PEo7KM55MJlrW6ASBNeMmf5blKLTHYjakWmNx2O86O7aPTm4wCe6JCKuy42rkijjGNHP4YI4FUdF9vwtuNHieADv0rRnh7OTKIPrRj+Jo4LbXwt+ryskfmNfWHDQgZLTDBwWOEbFohlZIb8kNzAbFohrGw2BaIblZDHeG5t60Q1ia61Xw3KyJZL1boUQgzBIOpbP7SfqnjIT715IemrqSnlldEeTabVRFKBeqojkmUq1ByyxArC5ZojlXLVSCPF6yRAcFe83rLEKrlrpCoqJSUFXto0reBM26SgBYbUIl52neo24qK5KoxRIEhbzIKtO64dvBBKoxReLb0gTRL10Fo147kC0YoMv79yBXB9ZzQh18nQB82lD8TG+4uM6RHTyhSDP+I/8AMV2/RZ8ZRAzClUf8QiA7gvn2esWtTYzsXuPeSVuzT8GryeFj81f9XjOAst5mg0DFB+jZxKDVhesZzRM2otaMee5K+8qM07F0SqMUxAlfzYq0+jv4LgFUYpnDXhuVad/AbkHbdFjvj4zQZ1oEcf0Xngt/SGKtDobdUQ98Vw/SvM6J3f8AHsb84Ob/ADNc3ivR6VHShURv2ZP80WIeK3Vn4E/fm8nJH52Pr/TgWXG1XQ5YrM02HsTscscPSvD0Gm61XsOtYmusCuY9WRLLercHa1dDfrWKtarWOU9s1qNgdrTVljD01exd2r5F5drSPdrVJeke5NpRQ5dbes7zrRrWrO9yjMr6VRxvt5msz9u9OXWHnSFme5VzLTSpXFRIVFBfoYhEzZpKgIts5mhEvO070G3FcSGYwRJErlWnNw7UEmMEXETuVaeJegLSMMdyBIwQZwO5AoPqnQ7TmNbHa9wbUqRvaIE+pcDIT0yJ7l85yvHD4z3XzcVmgxnNBquIngZKqattk3SKs+PBy5LX8zuIw5mgCMEH6NnEoNVTQckTuUaRglfeVGadi6JMYJpiV3NirT6O3yXBJjBFxGGG5Vpn8BuQdFmDTGwqfAe6wCI0kz0BwPBe30tx2fCIcFpDhBhtZMGYmBN1o1uK4SC6RBFlqESKXGbiScSrYyapys9sETmjJ5CCJFMHDDxVYuPYgCq1+mtrxZYrmRBgsZddsTteu7VWq317VYyIFiL7TtVjInFT2ptRrEQJusWOumESxd2hyNBiJXRAs9dB79wTbsUW9ZaqHvCUPtCpc9RmVtaHc8W2czVDnDDxUrX86VWVCZX1hCVEFFxJbEeZnaUA42qRG2naVA3mxAOsKJeZc6kKnNiJbZ+2pdA6wovcQUKnMwi9tv7II1537kC8qNbxwwULOZhcDB5klrlENs9QhU5mgLnGznSoHlRzbvMYoNbzYgLnmaLXlKW2n0Ra3mxAK5RrmU+dCWpzMJqtn7al0DrCmc8+A3JKnM0zm8MMEBY8zS1yixtv7JQzmYQMHWFDrCjVs/ZCogYvNigiFBzbvTWhVRxcYpn28UzIvHcqHC39kWjmxNuTVf1xTdcZdqzS5mEwFnbiF3aPKt644oPinwVMuZhR45sTbvLBxFMwkMQqNFvqEtVc2lowiG1L1hRDb/RCpzMLjpSVFCFEDRLztKgNh50oxJTO06PVQS5/dAk0xNg7eClnI9UTKQ54oEmmiXqWa+71RfKfPmgVht79yBTtlv0atqBlyPVBGmzuSpxKXp6pbNfd6oC83bOJQamdKznTtQEuR6oA68osN+xF0pny9VGy5/ddCTTaO08ELOR6prJc6ta4ETPO4bkLOR6pnS8Bu2oJDNqSasZKfPmls5HqggNhSzTiUjzxS2a+71QFxsHOlLNOZWJbOR6oC82oM4HcmfKfPmo2W/Rq2oEJTg2dqBlyPVMJbtHqgrBTRDuClmvu9UXS8MPVAIZtCVOyU/T1Qs5HqgjTfzpSp2yt54oWa+71QIoiVEDRLztO9RosPOkKKIETuFg7VFECJ4otUUQBg3HcgVFEDAWd3FIoogd+jZxKVt6iiAvvO1GGL9iiiBE+jtPBRRAid/AblFEEhC1IoogcCwpFFEDvFg2JFFEDRLypD4HcoogBTAWdo4qKIFCaJwCiiAQxaEqiiB2Cw86UiiiCKKKIP//Z\" width=\"150\" height=\"100\" ><h2>Nabız</h2><p>"+nabizDurumu+"</p></div></div></div></body></html>";
  return temp;
}

String veriAl()
{
  String temp = "<html lang=\"tr\"><head><meta charset=\"utf-8\"><body><p>"+nemOrani +"</p><p>"+ortamSicakligi +"</p><p>"+ vucutSicakligi +"</p><p>"+ co +"</p><p>"+ smoke +"</p><p>"+ nabizDurumu +"</p></body></html>";
  return temp;
}

SoftwareSerial swSer(13, 15, false, 256);
ESP8266WebServer server(80);
LiquidCrystal_I2C lcd(0x3F, 20, 4);
const int led = 13;

void SerialdenAl()
{
   if (swSer.read()=='a')
    {
       nemOrani=swSer.readStringUntil('*');
    }
    else if (swSer.read()=='b')
    {
       ortamSicakligi=swSer.readStringUntil('*');
    }
    else if (swSer.read()=='c')
    {
       vucutSicakligi=swSer.readStringUntil('*');
    }
    else if (swSer.read()=='d')
    {
        co=swSer.readStringUntil('*');
    } 
    else if (swSer.read()=='e')
    {
        smoke = swSer.readStringUntil('*');
    } 
    else if (swSer.read()=='f')
    {
        nabizDurumu = swSer.readStringUntil('*');
    }   
  }
void notification(String text)
{
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
   client.print("POST /api/pushes/");
                   client.print(CHANNEL);
                   client.println("/ HTTP/1.1");
                   client.print("Host: ");
                   client.println(host);
                   client.print("Authorization: Token ");
                   client.println(API);
                   client.println("Content-Type: application/json");
                   client.print("Content-Length: ");
                   client.println(text.length()+46);
                   client.println();
                   client.print("{ \"body\" : \"");
                   client.print(text);
                   client.println("\", \"message_type\" : \"text/plain\" }");
                   client.println(); 
  while(client.available()){
    String line = client.readStringUntil('\r');
  }
}

void handleRoot() {
  digitalWrite(led, 1); 
  SerialdenAl();
  server.send(200, "text/html", webPageText());
  digitalWrite(led, 0);
}

void handleNotFound(){
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void){
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");
  lcd.init();
  lcd.backlight();
  swSer.begin(115200);
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());
  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }
   lcd.setCursor(0,0);
  IPAddress ip = WiFi.localIP();
  String str = String(ip[0]);
    str += ".";
    str += String(ip[1]);
    str += ".";
    str += String(ip[2]);
    str += ".";
    str += String(ip[3]);  
    Serial.println(str);
    lcd.print(str);
  server.on("/", handleRoot); // anasayfaya girildiğinde çalışır
  server.on("/htmlsizgetir", [](){ //dataları html olmadan getirir
    server.send(200, "text/plain", "this works as well");
  });  
   server.on("/verial", [](){ //dataları html olmadan getirir
    server.send(200, "text/html", veriAl());
  });  
  server.onNotFound(handleNotFound); // 404 hatası
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
 server.handleClient();
   SerialdenAl();
   yenizaman = millis();
  String a = "";
  if((yenizaman-eskizaman)>= 120000){
    if(nemOrani != ""){
      if(nemOrani.toFloat() < 10 ) a += "Nem oranı :" + nemOrani + "Nem Oranı Çok Düşük.";  
      if(nemOrani.toFloat() > 40 ) a += "Nem oranı :" + nemOrani + "Nem Oranı Çok Yüksek.";
    }
    if(ortamSicakligi != ""){
      if(ortamSicakligi.toFloat() < 25 ) a +="Oda Sıcaklığı :" + ortamSicakligi + "Oda Sıcaklığı Çok Düşük.";
      if(ortamSicakligi.toFloat() > 35 )a +="Oda Sıcaklığı :" + ortamSicakligi + "Oda Sıcaklığı Çok Yüksek.";
    }
    if(vucutSicakligi != ""){
      if(vucutSicakligi.toFloat() < 35.5 ) a +="Vücut Sıcaklığı :" + vucutSicakligi + "Vücut Sıcaklığı Çok Düşük.";
      if(vucutSicakligi.toFloat() > 36.5 ) a +="Vücut Sıcaklığı :" + vucutSicakligi + "Vücut Sıcaklığı Çok Yüksek.";
    }
    if(co != "")
      if(co.toFloat() >  200 ) a +="Karbonmonoksit :" + co + "Karbonmonoksit miktarı çok yüksek";
    if(smoke != "")  
      if(smoke.toFloat()> 200 ) a +="Duman :" + smoke + "Duman miktarı çok yüksek";
    if(nabizDurumu != ""){
      if(nabizDurumu.toFloat() < 50 ) a +="Nabız :" + nabizDurumu + "Nabız Çok Düşük.";
      if(nabizDurumu.toFloat() > 100 )a +="Nabız :" + nabizDurumu + "Nabız Çok Yüksek.";
    }
    if(a != "") notification(a);
    eskizaman = yenizaman;
 }
}

