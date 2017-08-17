## CREATOR-Arduino-SDK

## Use guide
1.First: download the Arduino IDE.see the link :https://www.arduino.cc/en/Main/Software

2.Secend: download the CREATOR Arduino library. 
   Arduino IDE 1.6.5 support third party hardware by providing hardware configuration.
You need add Ameba's configuration in "File" -> "Preferences".
And fill below URL in "Additional Boards Manager URLs:"

   https://github.com/RAKWireless/CREATOR-Arduino-SDK/raw/master/release/package_rakwireless.com_creator_index.json

We also suggest to enable "Show verbose output" options on "compilation" and "upload" in Preference.

  Open "Device Manager" in "Tolls" -> "Board" -> "Board Manager".
Wait for IDE update core configuration. Schroll down the menu, you will see RAK CREATOR in the list.
Press "Install" at the right side.

  Select CREATOR in "Tools" -> "Board" -> "CREATOR RTL8711".
Now you are able to develop Arduino code and upload image onto Ameba.
