-    Zigbee Weather Station :  Get Humidity, Temperature, Luminosity

The purpose of the project is : to develop a low-power gadget,
that provide via Zigbee the Humidity, Temperature, Luminosity.
I also use 3310 Nokia LCD to display locally those information.

full documentation can be found at :
https://sites.google.com/site/nabilchouba2/home/zigbeeweatherstation


At the beginning of 2008 Zigbee is becoming very popular,
We think at that time that everything will have RF interface and that all manufacture
will play the game ... but still at this day nothing popup :(

the purpose of the project  is : to develop a low-power gadget,
that can provide via Zigbee the Humidity, Temperature, Luminosity.

Ti provide very good development kit EZ430-RF2500 ,
it uses the MSP430F2274 which combines 16-MIPS performance
with a 200-ksps 10-bit ADC and 2 op-amps and is paired
with the CC2500 multi-channel RF transceiver designed for low-power wireless applications.


the Temperature, Luminosity sensor : they change their internal resistance and the change
we use the ADC to get the needed value.

There is some Humidity sensor that provide digital interface as SPI or I2C for read & configuration
but there are too expensive and any way the sensor is analog and they need to integrate an ADC
and other circuit to implement the needed logic for conversion & SPI/I2C algorithm.
it better to work with analog sensor that are chipper, and implement all in the MSP430 microcontroller,
our MSP430 has already 2 very useful feature the10bit ADC and 2 op-amps that we can use for the jobs.

Humidity sensor is like a capacity that change flowing specific curve that depend on the sensor.
this mean that the project is manly to make a circuit that measure the capacity value.

The famous Timer 555 circuit can be use to make such measurement.
to understand how Timer 555 run best is to go on this page.




The charge time (output HIGH) is given by :
    t1 = 0.693 (R1 + R2) C1
and the discharge time (output LOW) by :
    t2 = 0.693 (R2) C1
Thus the total period T is given by :
    T = t1 + t2 = 0.693 (R1 + 2R2) C1
The frequency ofoscillation is them :
    F =1/T=1.44/ (R1 + 2R2) C1
=> source datasheet NE555
=> similar project Digital Capacitance Meter
Of course we don't need an other circuit (555) to get the capa value, and it can be done using only the MSP430
but it good to have a reference value as we are measuring very low capa value and if you bay one it 5-10% precise :

Measurement of the charge and discharge times of an RC circuit using Comparator_A+ and Timer_A of the MSP430



    Source : best book for MSP430 : MSP430 µC Basics
    We chose Vcaref = 1/2 Vdd, to get charge and discharge time equal.
    Tdischarge = RC(log(2))   => C  = Tdis / R log(2)
    We also perform 100 measurement on MSP430 to remove noise error on the result.

Experimental result on capa measurement comparing Timer 555 and MSP430 :
    we have 10 capa : all result and formula on Excel File




 capa	 33p A	 33p B	 101p	 3113p	 1000p	 free
 T555	 33,19	 34,21	 103,84	 3140	1048	 0 (ref)
 MSP	 32,98	 33,73	 103,69	 3175	 1035	 0 (ref)


   we will measure capa value of the sensor that vary from 280p to 380p
   we will use : DSHS07 datasheet humidity sensor


For the photoresistor and temperature sensor : you can find docuemantation on website Societyofrobots
we only need to use a reference Resistance and the ADC of the MSP430
we need to build : Voltage Divider Circuits
Vout  = (Rphoto*Vin)/(Rphoto+R)



Sister board : the 3 sensor :



To add more fun on this sad project that contain only Resistance an Capacity Measurement
we decide to add the legendary LCD screen of 3310 Nokia it cost only 2$ and give GUI to the project
by this way not only engineer will appreciate the project :P and sure Steven Jobs will make same choice :P



the Nokia 3310 screen use SPI like interface and is very simple and intuitive to program.
as you see in figure the screen run in loop mode every 2 sec it show (instant resume, humidity or temperature or luminosity curve for 24h)

finally for Zigbee all is provided by Ti ... you only have to type send data :D
