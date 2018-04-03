/* pinexist.h genetated by MarlinPinExist.exe */
#if !defined(__ARM_LPC1768__) 
#endif
#if defined(LED_PIN) && LED_PIN >= 0
	 #define LED_PIN_EXIST 1
#else
	 #define LED_PIN_EXIST 0
#endif

#if defined(LED1_PIN) && LED1_PIN >= 0
	 #define LED1_PIN_EXIST 1
#else
	 #define LED1_PIN_EXIST 0
#endif

#if ENABLED(DELTA)
#if defined(X_MAX_PIN) && X_MAX_PIN >= 0
	 #define X_MAX_PIN_EXIST 1
#else
	 #define X_MAX_PIN_EXIST 0
#endif

#if defined(Y_MAX_PIN) && Y_MAX_PIN >= 0
	 #define Y_MAX_PIN_EXIST 1
#else
	 #define Y_MAX_PIN_EXIST 0
#endif

#if defined(Z_MAX_PIN) && Z_MAX_PIN >= 0
	 #define Z_MAX_PIN_EXIST 1
#else
	 #define Z_MAX_PIN_EXIST 0
#endif

#if defined(Z_MIN_PIN) && Z_MIN_PIN >= 0
	 #define Z_MIN_PIN_EXIST 1
#else
	 #define Z_MIN_PIN_EXIST 0
#endif

#else
  #ifdef IS_RAMPS_LPC1768
#if defined(X_MAX_PIN) && X_MAX_PIN >= 0
	 #define X_MAX_PIN_EXIST 1
#else
	 #define X_MAX_PIN_EXIST 0
#endif

#if defined(Y_MAX_PIN) && Y_MAX_PIN >= 0
	 #define Y_MAX_PIN_EXIST 1
#else
	 #define Y_MAX_PIN_EXIST 0
#endif

#if defined(Z_MAX_PIN) && Z_MAX_PIN >= 0
	 #define Z_MAX_PIN_EXIST 1
#else
	 #define Z_MAX_PIN_EXIST 0
#endif

  #endif
#if defined(X_MIN_PIN) && X_MIN_PIN >= 0
	 #define X_MIN_PIN_EXIST 1
#else
	 #define X_MIN_PIN_EXIST 0
#endif

#if defined(Y_MIN_PIN) && Y_MIN_PIN >= 0
	 #define Y_MIN_PIN_EXIST 1
#else
	 #define Y_MIN_PIN_EXIST 0
#endif

#if defined(Z_MIN_PIN) && Z_MIN_PIN >= 0
	 #define Z_MIN_PIN_EXIST 1
#else
	 #define Z_MIN_PIN_EXIST 0
#endif

#endif
#if defined(X_STEP_PIN) && X_STEP_PIN >= 0
	 #define X_STEP_PIN_EXIST 1
#else
	 #define X_STEP_PIN_EXIST 0
#endif

#if defined(X_DIR_PIN) && X_DIR_PIN >= 0
	 #define X_DIR_PIN_EXIST 1
#else
	 #define X_DIR_PIN_EXIST 0
#endif

#if defined(X_ENABLE_PIN) && X_ENABLE_PIN >= 0
	 #define X_ENABLE_PIN_EXIST 1
#else
	 #define X_ENABLE_PIN_EXIST 0
#endif

#if defined(Y_STEP_PIN) && Y_STEP_PIN >= 0
	 #define Y_STEP_PIN_EXIST 1
#else
	 #define Y_STEP_PIN_EXIST 0
#endif

#if defined(Y_DIR_PIN) && Y_DIR_PIN >= 0
	 #define Y_DIR_PIN_EXIST 1
#else
	 #define Y_DIR_PIN_EXIST 0
#endif

#if defined(Y_ENABLE_PIN) && Y_ENABLE_PIN >= 0
	 #define Y_ENABLE_PIN_EXIST 1
#else
	 #define Y_ENABLE_PIN_EXIST 0
#endif

#if defined(Z_STEP_PIN) && Z_STEP_PIN >= 0
	 #define Z_STEP_PIN_EXIST 1
#else
	 #define Z_STEP_PIN_EXIST 0
#endif

#if defined(Z_DIR_PIN) && Z_DIR_PIN >= 0
	 #define Z_DIR_PIN_EXIST 1
#else
	 #define Z_DIR_PIN_EXIST 0
#endif

#if defined(Z_ENABLE_PIN) && Z_ENABLE_PIN >= 0
	 #define Z_ENABLE_PIN_EXIST 1
#else
	 #define Z_ENABLE_PIN_EXIST 0
#endif

#if defined(E0_STEP_PIN) && E0_STEP_PIN >= 0
	 #define E0_STEP_PIN_EXIST 1
#else
	 #define E0_STEP_PIN_EXIST 0
#endif

#ifdef DELTA
#if defined(E0_DIR_PIN) && E0_DIR_PIN >= 0
	 #define E0_DIR_PIN_EXIST 1
#else
	 #define E0_DIR_PIN_EXIST 0
#endif

#else
#if defined(E0_DIR_PIN) && E0_DIR_PIN >= 0
	 #define E0_DIR_PIN_EXIST 1
#else
	 #define E0_DIR_PIN_EXIST 0
#endif

#endif
#if defined(E0_ENABLE_PIN) && E0_ENABLE_PIN >= 0
	 #define E0_ENABLE_PIN_EXIST 1
#else
	 #define E0_ENABLE_PIN_EXIST 0
#endif

#ifdef DELTA
#else
#endif
#if defined(LEVEL_0_PIN) && LEVEL_0_PIN >= 0
	 #define LEVEL_0_PIN_EXIST 1
#else
	 #define LEVEL_0_PIN_EXIST 0
#endif

#if defined(LEVEL_1_PIN) && LEVEL_1_PIN >= 0
	 #define LEVEL_1_PIN_EXIST 1
#else
	 #define LEVEL_1_PIN_EXIST 0
#endif

#if defined(TEMP_0_PIN) && TEMP_0_PIN >= 0
	 #define TEMP_0_PIN_EXIST 1
#else
	 #define TEMP_0_PIN_EXIST 0
#endif

#if defined(TEMP_BED_PIN) && TEMP_BED_PIN >= 0
	 #define TEMP_BED_PIN_EXIST 1
#else
	 #define TEMP_BED_PIN_EXIST 0
#endif

#if ENABLED(IS_RAMPS_EFB)      // Hotend, Fan, Bed
#if defined(HEATER_0_PIN) && HEATER_0_PIN >= 0
	 #define HEATER_0_PIN_EXIST 1
#else
	 #define HEATER_0_PIN_EXIST 0
#endif

#else
#if defined(HEATER_0_PIN) && HEATER_0_PIN >= 0
	 #define HEATER_0_PIN_EXIST 1
#else
	 #define HEATER_0_PIN_EXIST 0
#endif

#if defined(HEATER_BED_PIN) && HEATER_BED_PIN >= 0
	 #define HEATER_BED_PIN_EXIST 1
#else
	 #define HEATER_BED_PIN_EXIST 0
#endif

#endif
#if defined(DEBUG_PIN) && DEBUG_PIN >= 0
	 #define DEBUG_PIN_EXIST 1
#else
	 #define DEBUG_PIN_EXIST 0
#endif

#if defined(TMP03_0_PIN) && TMP03_0_PIN >= 0
	 #define TMP03_0_PIN_EXIST 1
#else
	 #define TMP03_0_PIN_EXIST 0
#endif

#if defined(TMP03_1_PIN) && TMP03_1_PIN >= 0
	 #define TMP03_1_PIN_EXIST 1
#else
	 #define TMP03_1_PIN_EXIST 0
#endif

#if defined(HEATER_PIN) && HEATER_PIN >= 0
	 #define HEATER_PIN_EXIST 1
#else
	 #define HEATER_PIN_EXIST 0
#endif

#if defined(RED_LASER_PIN) && RED_LASER_PIN >= 0
	 #define RED_LASER_PIN_EXIST 1
#else
	 #define RED_LASER_PIN_EXIST 0
#endif

#if defined(ZATVOR_PIN) && ZATVOR_PIN >= 0
	 #define ZATVOR_PIN_EXIST 1
#else
	 #define ZATVOR_PIN_EXIST 0
#endif

