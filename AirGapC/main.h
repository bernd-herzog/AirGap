void AirGap_main();

#define CONNECT(source ,target) source ## _ ## ReportData = &target ## _ ## OnData
