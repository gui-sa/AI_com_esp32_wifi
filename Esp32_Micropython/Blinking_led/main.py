while(1):
	led.value(1)			#Setting led to high
	time.sleep_ms(1000)      # sleep for 500 milliseconds
	led.value(0)			#Setting led to low
	time.sleep_ms(1000)      # sleep for 500 milliseconds
