
void spi_bus_init(void){
    spi_bus_config_t bus = {
        .mosi_io_num = 23, .miso_io_num = 19,    // SDI y SDO
        .sclk_io_num = 18, .quadwp_io_num = -1, .quadhd_io_num = -1,
    };
    spi_bus_initialize(SPI2_HOST, &bus, SPI_DMA_CH_AUTO);
    spi_device_interface_config_t dev = {
        .clock_speed_hz = 1000000,   
        .mode = 0,                    
        .spics_io_num = 5,            
        .queue_size = 1,
    };
    spi_bus_add_device(SPI2_HOST, &dev, &mcp);
}

void mcp4132_write_register(uint8_t addr, uint16_t valor){
    uint8_t cmd_byte  = (addr << 4) | (0b00 << 2) | ((valor >> 8) & 0x03); 
    uint8_t data_byte = valor & 0xFF;                                      
    uint8_t buf[2] = { cmd_byte, data_byte };
    spi_transaction_t t = { .length = 16, .tx_buffer = buf };              s
    spi_device_transmit(mcp, &t);
}


uint16_t mcp4132_read_register(uint8_t addr){
    uint8_t tx[2] = { (addr << 4) | (0b11 << 2), 0x00 };  
    uint8_t rx[2] = { 0 };
    spi_transaction_t t = { .length = 16, .tx_buffer = tx, .rx_buffer = rx };
    spi_device_transmit(mcp, &t);
    uint16_t resp = ((uint16_t)rx[0] << 8) | rx[1];
    return resp & 0x01FF;                                  
}

void mcp4132_set_wiper(uint16_t N){
    if (N > 128) N = 128;                                  
    mcp4132_write_register(0x00, N);                       
}


void mcp4132_set_cutoff_frequency(float fc_hz){
    float R   = 1.0f / (2.0f * M_PI * fc_hz * MCP_CAP);    
    float N_f = (R - MCP_RW) * 128.0f / MCP_RAB;          
    int   N   = (int)(N_f + 0.5f);                         
    if (N < 0)   N = 0;                                    
    if (N > 128) N = 128;                                  
    mcp4132_set_wiper((uint16_t)N);
}