# pac-man-ghost-lamp
MQTT over wifi (using ESP8266) controlled night lamp with RGB light effects.

Originally intended to be used as user activity monitor for Qlik Sense enterprise environments, but given that the lamp is controlled by MQTT messages, it can be used in any MQTT setting.

Copy the src/secrets_template.h file to a file named src/secrets.h, then edit the new file as needed wrt  
* the wifi network the lamp will be used in, and
* the IP address of the MQTT broker to be used  

More extensive blog post over at [Ptarmigan Labs](https://ptarmiganlabs.com/blog/2017/05/01/let-there-be-blinky-light/).
