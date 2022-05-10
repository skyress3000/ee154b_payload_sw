// functions

// function to read radio commands from serial and handle them. Should be called by main loop
void read_radio();

// constants

#define ACK_BYTE    0xFF // response indicating a command has been received
#define ERR_BYTE    0x0F // response indicating some error with a command (command not found or length does not match data bytes)
#define DONE_BYTE   0xF0 // response indicating a command has finished running and response data will follow
#define STAT_BYTE   0x55 // byte indicating payload status message

// typedefs

typedef struct {
    const uint8_t command_byte;                                                 // byte sent over radio to run this command
    uint8_t (*run_command)(uint8_t* args, int args_len, uint8_t* resp_buff);    // function to run this command
                                                                                // args: char sequence following command to be interpreted by function
                                                                                // args_len: number of bytes in args
                                                                                // resp_buff: buffer to write response into
                                                                                // return value: length of data in resp_buff
} command_t;