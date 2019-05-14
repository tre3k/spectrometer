/* Machine.java (c) Kirill Pshenichnyi PNPI 2019
   just class for work with hardware, usb, etc
   This function aslo as readMemory, Init, ...
   This is not servlet!
 */

import org.usb4java.Context;
import org.usb4java.DeviceHandle;
import org.usb4java.LibUsb;
import org.usb4java.LibUsbException;

import java.nio.ByteBuffer;
import java.nio.IntBuffer;
import java.util.concurrent.TimeUnit;


public class Machine {

    public int[] memory;

    private Context context;
    private DeviceHandle deviceHandle;
    private int result;
    /*
    private static final short VID = (short)0x04b4;
    private static final short PID = (short)0x8613;
    */
    private static final short VID = (short)0x0930;
    private static final short PID = (short)0x6544;

    /* defice hardware commands */
    private static final byte CMD_TEST = 0x00;
    private static final byte CMD_INIT_HARDWARE = 0x01;
    private static final byte CMD_CLEAR_MEM = 0x02;
    private static final byte CMD_START_ADC = 0x03;
    private static final byte CMD_STOP_ADC = 0x04;
    private static final byte CMD_READ_DATA = 0x05;

    private static final byte CMD_SET_TOF_DELAY = 0x15;
    private static final byte CMD_SET_TOF_CHAN_WIDTH = 0x16;
    private static final byte CMD_SET_TOF_N_CHANNELS = 0x17;
    private static final byte CMD_TOF_START = 0x18;
    private static final byte CMD_TOF_STOP = 0x19;
    private static final byte CMD_MEMORY_ZONE = 0x1a;

    /* Constructor */
    Machine(){
        /* create USB connection */

        /* init libusb */
        context = new Context();

        result = LibUsb.init(context);
        if(result != LibUsb.SUCCESS) throw new LibUsbException("Unable to initialize libusb.", result);

        /* open device_handle */
        deviceHandle = LibUsb.openDeviceWithVidPid(context,VID,PID);
        if(deviceHandle == null) throw new LibUsbException("Error open device!",0);

        /* driver activate */
        result = LibUsb.kernelDriverActive(deviceHandle,0);
        if(result == 1){
            result = LibUsb.detachKernelDriver(deviceHandle,0);
            if(result != 0) throw new LibUsbException("Error detach kernel driver!",result);
        }

        /* claim interface */
        result = LibUsb.claimInterface(deviceHandle,0);
        if(result < 0) throw  new LibUsbException("Error claim interface!", result);

        memory = new int [16384];   // 64kB of memory
     }

    /* close USB connection */
    public void close(){
        /* release and close USB interface */
        LibUsb.releaseInterface(deviceHandle,0);
        LibUsb.close(deviceHandle);
        deviceHandle = null;
        context = null;
    }

    /* hardware initialization */
    public void Init(){
        sendCommand(CMD_INIT_HARDWARE, (byte)0, (byte)0);
        sendCommand(CMD_CLEAR_MEM, (byte)0, (byte)0);
    }

    /* start command */
    public void Start(int channels_code, int width_code, int delay_code){
        /* set TOF number channels */
        sendCommand(CMD_SET_TOF_N_CHANNELS,(byte)(channels_code & 0xff), (byte) 0x00);
        /* set TOF width channels */
        sendCommand(CMD_SET_TOF_CHAN_WIDTH, (byte)(width_code & 0xff), (byte) 0x00);
        /* set TOF delay */
        sendCommand(CMD_SET_TOF_DELAY, (byte)(delay_code & 0xff), (byte)((delay_code & 0xff00)>>8));
        /* start! */
        sendCommand(CMD_TOF_START,(byte)0, (byte)0);
    }

    /* stop command */
    public void Stop(){
        sendCommand(CMD_TOF_STOP,(byte)0, (byte)0);
    }

    public void ReadMem(){
        /* set first memory zone */
        //sendCommand(CMD_MEMORY_ZONE,(byte)0, (byte)0);
        // add here function for read memory and insert data to memory[]


        /* set two memory zone*/
        //sendCommand(CMD_MEMORY_ZONE,(byte)1, (byte)0);
        // add here function for read memory and insert data to memory[]


        for(int i=0;i<16384;i++){
            memory[i] = i*2;
        }

    }

    /* function for send 64 byte and recevery */
    private int sendCommand(byte command,byte b1, byte b2){
        final int PACKET_SIZE = 64;
        final long TIMEOUT = 200;
        final IntBuffer actual = IntBuffer.allocate(1);
        ByteBuffer buffer = ByteBuffer.allocate(PACKET_SIZE);
        buffer.put(0,command);
        buffer.put(1,b1);
        buffer.put(2,b2);

        /* send command */
        result = LibUsb.bulkTransfer(deviceHandle,(byte)(LibUsb.ENDPOINT_OUT | 1), buffer, actual, TIMEOUT);
        if(result < 0) throw new LibUsbException("Error send data!", result);

        /* wait for clear memory in device */
        //if(command == CMD_CLEAR_MEM) TimeUnit.MILLISECONDS.sleep(500);
        try {
            Thread.sleep(1);
        }catch (InterruptedException ex){
            Thread.currentThread().interrupt();
        }

        /* recevery answer: 0x01 */
        result = LibUsb.bulkTransfer(deviceHandle,(byte)(LibUsb.ENDPOINT_IN | 1), buffer, actual, TIMEOUT);
        if(result < 0) throw new LibUsbException("Error recv data!", result);

        if(buffer.get(0)!=0x01) return -1;
        return 0;
    }


}
