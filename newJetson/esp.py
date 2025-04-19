from serial import Serial    
def connect(self):
        print("Connecting to Microcontroller on port", self.port, "...")
        while True:
            try:
                self.port = Serial(port=self.port, baudrate=self.baudrate, timeout=self.timeout, writeTimeout=self.writeTimeout)
                break
            except Exception:
                print("Intent connect fail")
        # The next line is necessary to give the firmware time to wake up.
        time.sleep(0.5)
        state_, val = self.get_baud()
        while val != self.baudrate:
            state_, val = self.get_baud()
            time.sleep(0.5)
            print("Intent connect fail")
        print("Connected at", self.baudrate)
        print("Microcontroller is ready.")



    def connect(self):
        while True:
            try:
                print("Connecting to Microcontroller on port", self.port, "...")
                self.port = Serial(port=self.port, baudrate=self.baudrate, timeout=self.timeout, writeTimeout=self.writeTimeout)
                # The next line is necessary to give the firmware time to wake up.
                time.sleep(1)
                state_, val = self.get_baud()
                if val != self.baudrate:
                    time.sleep(1)
                    state_, val  = self.get_baud()
                    if val != self.baudrate:
                        raise SerialException
                print("Connected at", self.baudrate)
                print("Microcontroller is ready.")
                break

            except SerialException:
                print("Serial Exception:")
                print(sys.exc_info())
                print("Traceback follows:")
                traceback.print_exc(file=sys.stdout)
                print("Cannot connect to Microcontroller!")
                os._exit(1)