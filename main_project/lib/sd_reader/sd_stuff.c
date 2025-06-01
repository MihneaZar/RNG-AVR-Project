// void SD_Init() {
//     uint32_t last_attempt = 0;
//     while (1) {
//         printf("about to mount\n");
//         uint8_t mount = pf_mount(&fs);
//         printf("done mounting\n");
//         printf("%d\n", mount);
//         if (mount == FR_OK) {
//             printf("mounting is successful\n");
//             break;
//         }
//         // Waits for a second before retrying
//         while(!SYSTICKS_PASSED(last_attempt, 1000));
//         last_attempt = systicks;
//     }
//     pf_open("log.csv");
//     pf_lseek(0);
// }

// void SD_log_data(uint32_t temp_c) {
//     /* TODO4 Scrieti temp_c in log.csv */
//     WORD w;
//     pf_write(&temp_c, 4, &w);
//     pf_write(NULL, 0, &w);
// }

// uint32_t SD_read_data() {
//     uint32_t temperature_c;
//     WORD w;
//     pf_read(&temperature_c, 4, &w);
//     return temperature_c;
// }