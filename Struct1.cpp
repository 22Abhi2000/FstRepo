
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1800
#define INPUT_FILE "Chaser_MeasurementGeneration.out"
#define OUTPUT_FILE "output.hex"
#define Pos_Y -350558.5530
#define NUM_GPS_CHANNEL 16
#define TX_Buffer_Size 1800
#define uint8_t unsigned char
#define uint16_t unsigned short
#define uint32_t unsigned int

FILE *fp2;
uint16_t uttemp_wk_num = 0;
uint8_t msmt_counter = 0;
unsigned char TxBuffer1[TX_Buffer_Size] = {0};

typedef struct {
    unsigned char TxBuffer[BUFFER_SIZE];
} UART;

static uint16_t to_be16(uint16_t x) { return (x >> 8) | (x << 8); }

static uint32_t to_be32(uint32_t x) {
    return ((x>>24)&0xFF) | ((x>>8)&0xFF00) |
           ((x<<8)&0xFF0000) | ((x<<24)&0xFF000000);
}

static uint64_t to_be64(uint64_t x) {
    return ((x>>56)&0xFFULL) |
           ((x>>40)&0xFF00ULL) |
           ((x>>24)&0xFF0000ULL) |
           ((x>>8)&0xFF0000000ULL) |
           ((x<<8)&0xFF00000000ULL) |
           ((x<<24)&0xFF0000000000ULL) |
           ((x<<40)&0xFF000000000000ULL) |
           ((x<<56)&0xFF00000000000000ULL);
}

#pragma pack(push,1)
typedef struct {
    uint16_t header; /* 0xACCA */
    uint8_t msmt_counter; /* UINT8 */
    uint8_t nav_state; /* UINT8 */
    uint8_t antenna_status; /* UINT8 */
    uint8_t pvt_status; /* UINT8 */
    uint8_t gps_nav_sats; /* UINT8 */
    uint8_t navic_sats; /* UINT8 */
    uint16_t GPS_WK_Num;
    double IDGps_Tow;
    double IDRec_Pos[3];
    float IDRec_Vel[3];
    float IDGdop_Pdop[2];
    uint16_t Dlt_Time[2];
    uint16_t Checksum;
    uint16_t Nav_Week_Num;
    double IDNav_Tow;
    uint8_t Type_22_Data[37];
    uint8_t Sv_status[16];
    uint8_t Sv_id[16];
    uint8_t Cndr[16];
    double IDPR[16];
    double IDDR[16];
    double IDCR[16];
    double IDBias;
    double IDDrift;
    uint8_t Ephermeris[71];
    uint16_t Msg_Rec_Ctr;
    uint8_t Debug_info[22];
    uint8_t SV_ID[14];
    uint8_t CNDR[14];
    double IDPseudo_Range[4];
    double IDDelta_Range[4];
    double IDGPSSV_Pos[NUM_GPS_CHANNEL][7];
    uint8_t Elevation[NUM_GPS_CHANNEL];
    uint16_t Azimuth[NUM_GPS_CHANNEL];
    uint16_t Uart_Tx_Ctr;
    uint8_t Ast_Rst_Ctr;
    uint8_t Ast_Rst_id;
    uint8_t Ast_Debug[100];
    uint16_t Checksum1;
} GPSPacketInp;

#pragma pack(pop)
GPSPacketInp GLStGPSPacketInp;

void write_u16_be(unsigned char **p, uint16_t v) {
    v = to_be16(v);
    memcpy(*p, &v, 2);
    *p += 2;
}

void write_double_be(unsigned char **p, double d) {
    uint64_t tmp;
    memcpy(&tmp, &d, 8);
    tmp = to_be64(tmp);
    memcpy(*p, &tmp, 8);
    *p += 8;
}

void write_float_be(unsigned char **p, float f) {
    uint32_t tmp;
    memcpy(&tmp, &f, 4);
    tmp = to_be32(tmp);
    memcpy(*p, &tmp, 4);
    *p += 4;
}

FILE *fin = fopen(INPUT_FILE, "rt+");

void Convert_Eng_data2Hex() {
    uint16_t uiCHksum = 0;
    
    // Initialize struct and buffer
    memset(&GLStGPSPacketInp, 0, sizeof(GPSPacketInp));
    memset(TxBuffer1, 0, sizeof(TxBuffer1));
    
    // Check file opening
    if (fin == NULL) {
        printf("Error opening input file\n");
        return;
    }
    
    // Read basic information
    fscanf(fin, "%d %lf", &GLStGPSPacketInp.GPS_WK_Num, &GLStGPSPacketInp.IDGps_Tow);
    uttemp_wk_num = GLStGPSPacketInp.GPS_WK_Num;
    
    // Read position data
    for (int i = 0; i < 3; i++) {
        fscanf(fin, "%lf ", &GLStGPSPacketInp.IDRec_Pos[i]);
    }
    
    // Read velocity data
    for (int i = 0; i < 3; i++) {
        fscanf(fin, "%f ", &GLStGPSPacketInp.IDRec_Vel[i]);
    }
    
    // Read satellite information
    fscanf(fin, "%02x", &GLStGPSPacketInp.gps_nav_sats);
    
    for (int i = 0; i < 16; i++) {
        fscanf(fin, "%02d", &GLStGPSPacketInp.Sv_id[i]);
        if(GLStGPSPacketInp.Sv_id[i] != 00) {
            fscanf(fin, "%lf %lf %lf %02d %lf %lf %lf %lf %lf %lf",
                   &GLStGPSPacketInp.IDPR[i], &GLStGPSPacketInp.IDCR[i],
                   &GLStGPSPacketInp.IDDR[i], &GLStGPSPacketInp.Cndr[i], 
                   &GLStGPSPacketInp.IDGPSSV_Pos[i][0], &GLStGPSPacketInp.IDGPSSV_Pos[i][1], 
                   &GLStGPSPacketInp.IDGPSSV_Pos[i][2], &GLStGPSPacketInp.IDGPSSV_Pos[i][3], 
                   &GLStGPSPacketInp.IDGPSSV_Pos[i][4], &GLStGPSPacketInp.IDGPSSV_Pos[i][5]);
        }
    }
    
    // Set header and counters
    GLStGPSPacketInp.GPS_WK_Num = uttemp_wk_num;
    GLStGPSPacketInp.header = 0xcaac;
    
    if (GLStGPSPacketInp.msmt_counter == 0)
        GLStGPSPacketInp.msmt_counter = 1;
    
    if (GLStGPSPacketInp.Uart_Tx_Ctr == 0)
        GLStGPSPacketInp.Uart_Tx_Ctr = 1;
    
    // Copy struct to buffer
    memcpy(TxBuffer1, &GLStGPSPacketInp, sizeof(GPSPacketInp));
    
    // Calculate checksum only over the actual data
    uiCHksum = 0;
    for (int i = 0; i < sizeof(GPSPacketInp); i++) {
        uiCHksum += TxBuffer1[i];
    }
    
    // Write checksum (little-endian)
    TxBuffer1[1798] = uiCHksum & 0xFF;
    TxBuffer1[1799] = (uiCHksum >> 8) & 0xFF;
    
    // Write to output file
    for (int i = 0; i < 1800; i++) {
        fprintf(fp2, "%02x\t", TxBuffer1[i]);
    }
    fprintf(fp2, "\n");
}

int main() {
    // Open output file
    fp2 = fopen(OUTPUT_FILE, "w");
    if (fp2 == NULL) {
        printf("Error opening output file\n");
        return -1;
    }
    
    // Process data
    Convert_Eng_data2Hex();
    
    // Close files
    fclose(fin);
    fclose(fp2);
    
    return 0;
}
