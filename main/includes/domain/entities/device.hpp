
typedef struct _device
{
     uint64_t id;
     string software_version;
     string hardware_version;
     switch_output_t * listOfSwitchsOnDevice;
     /*Quantity of Network interfaces in use
     Quantity of output gpios pins in use
     */
}gpio_switch_t;
