/**
 * SILENT-001 Firmware - Main Application
 * EMG-based Silent Communication System
 * 
 * Target: STM32L476RG (Nucleo-64)
 * Features:
 *  - 4-channel EMG ADC with DMA
 *  - Simple threshold-based gesture detection
 *  - UART command output
 *  - Low-power operation (~5mA active)
 * 
 * Author: SILENT Research Team
 * Version: 0.1.0
 */

#include "stm32l4xx_hal.h"
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

/* ============================================================================
 * CONFIGURATION AND CONSTANTS
 * ============================================================================ */

#define FIRMWARE_VERSION "0.1.0"
#define SYSTEM_CLOCK_HZ 80000000U

/* ADC Configuration */
#define ADC_NUM_CHANNELS        4
#define ADC_SAMPLES_PER_CHANNEL 128
#define ADC_BUFFER_SIZE         (ADC_NUM_CHANNELS * ADC_SAMPLES_PER_CHANNEL)
#define ADC_SAMPLE_RATE_HZ      1000
#define ADC_DMA_PRIORITY        DMA_PRIORITY_HIGH

/* EMG Thresholds (raw ADC values, 12-bit = 0-4095) */
#define EMG_THRESHOLD_LOW       2200      /* ~1.0V (midpoint + noise margin) */
#define EMG_THRESHOLD_HIGH      2600      /* ~1.2V (significant activation) */
#define EMG_HYSTERESIS          100       /* Anti-chatter hysteresis */
#define EMG_MIN_ACTIVE_SAMPLES  10        /* Debounce: min samples above thresh */

/* Command Detection */
#define CMD_BUFFER_SIZE         32
#define CMD_HOLD_TIME_MS        200       /* Minimum hold for command valid */
#define CMD_COOLDOWN_MS         500       /* Debounce between commands */

/* UART Configuration */
#define UART_BAUDRATE           115200
#define UART_TX_BUF_SIZE        64

/* GPIO Pin Definitions */
#define STATUS_LED_PIN          GPIO_PIN_5
#define STATUS_LED_PORT         GPIOA
#define EMG_ACTIVE_PIN          GPIO_PIN_6
#define EMG_ACTIVE_PORT         GPIOA

/* ============================================================================
 * GLOBAL VARIABLES
 * ============================================================================ */

/* Hardware Handles */
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;
UART_HandleTypeDef huart2;
TIM_HandleTypeDef htim2;

/* DMA Buffer - Must be aligned for DMA */
static volatile uint16_t adc_dma_buffer[ADC_BUFFER_SIZE] __attribute__((aligned(16)));
static volatile uint32_t adc_dma_complete = 0;
static volatile uint32_t adc_half_complete = 0;

/* Processed channel data */
static volatile uint16_t emg_channels[ADC_NUM_CHANNELS];
static volatile uint8_t emg_active[ADC_NUM_CHANNELS];
static volatile uint32_t emg_activity_start[ADC_NUM_CHANNELS];

/* Command State */
typedef enum {
    CMD_STATE_IDLE = 0,
    CMD_STATE_DETECTING,
    CMD_STATE_CONFIRMED,
    CMD_STATE_COOLDOWN
} CmdState_t;

static struct {
    CmdState_t state;
    uint32_t channel_mask;
    uint32_t start_time;
    uint32_t last_trigger;
    char cmd_buffer[CMD_BUFFER_SIZE];
} cmd_state;

/* System Timing */
static volatile uint32_t system_tick_ms = 0;

/* ============================================================================
 * FUNCTION PROTOTYPES
 * ============================================================================ */

/* System Init */
static void SystemClock_Config(void);
static void GPIO_Init(void);
static void DMA_Init(void);
static void ADC_Init(void);
static void TIM_Init(void);
static void UART_Init(void);

/* EMG Processing */
static void Process_EMG_Data(void);
static void Update_Channel_Status(void);
static void Detect_Commands(void);

/* Command Output */
static void Send_Command(const char* cmd);
static void UART_SendString(const char* str);

/* Utility */
static uint32_t GetTick_ms(void);
static uint16_t Calculate_Mean(uint16_t* data, uint32_t len);

/* ============================================================================
 * INITIALIZATION FUNCTIONS
 * ============================================================================ */

/**
 * @brief Configure system clock to 80MHz from HSI16
 */
static void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /* Enable Power Control clock and configure voltage scaling */
    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWR_EnableBkUpAccess();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /* Configure LSE Drive Capability */
    __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

    /* Initialize HSI at 16MHz and PLL to reach 80MHz */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 1;
    RCC_OscInitStruct.PLL.PLLN = 10;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    /* Configure bus clocks */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4);

    /* Update system clock */
    SystemCoreClock = SYSTEM_CLOCK_HZ;
}

/**
 * @brief Initialize GPIO pins
 */
static void GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Enable GPIO clocks */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    /* Status LED - PA5 (User LED on Nucleo) */
    HAL_GPIO_WritePin(STATUS_LED_PORT, STATUS_LED_PIN, GPIO_PIN_RESET);
    GPIO_InitStruct.Pin = STATUS_LED_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(STATUS_LED_PORT, &GPIO_InitStruct);

    /* EMG Active Indicator - PA6 */
    HAL_GPIO_WritePin(EMG_ACTIVE_PORT, EMG_ACTIVE_PIN, GPIO_PIN_RESET);
    GPIO_InitStruct.Pin = EMG_ACTIVE_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(EMG_ACTIVE_PORT, &GPIO_InitStruct);

    /* ADC Input Pins - PA0, PA1, PA3, PA4 (AIN0, AIN1, AIN3, AIN4) */
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_3 | GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/**
 * @brief Initialize DMA for ADC
 */
static void DMA_Init(void)
{
    /* Enable DMA1 clock */
    __HAL_RCC_DMA1_CLK_ENABLE();

    /* Configure DMA for ADC1 */
    hdma_adc1.Instance = DMA1_Channel1;
    hdma_adc1.Init.Request = DMA_REQUEST_ADC1;
    hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_adc1.Init.Mode = DMA_CIRCULAR;
    hdma_adc1.Init.Priority = ADC_DMA_PRIORITY;

    HAL_DMA_Init(&hdma_adc1);

    /* Link DMA to ADC */
    __HAL_LINKDMA(&hadc1, DMA_Handle, hdma_adc1);

    /* Enable DMA interrupts */
    HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}

/**
 * @brief Initialize ADC for 4-channel continuous conversion with DMA
 */
static void ADC_Init(void)
{
    ADC_ChannelConfTypeDef sConfig = {0};
    ADC_MultiModeTypeDef multimode = {0};

    /* Enable ADC clock */
    __HAL_RCC_ADC_CLK_ENABLE();

    /* Configure ADC1 */
    hadc1.Instance = ADC1;
    hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
    hadc1.Init.Resolution = ADC_RESOLUTION_12B;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
    hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;
    hadc1.Init.LowPowerAutoWait = DISABLE;
    hadc1.Init.ContinuousConvMode = ENABLE;
    hadc1.Init.NbrOfConversion = ADC_NUM_CHANNELS;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc1.Init.DMAContinuousRequests = ENABLE;
    hadc1.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
    hadc1.Init.OversamplingMode = DISABLE;

    HAL_ADC_Init(&hadc1);

    /* Configure multi-mode (independent) */
    multimode.Mode = ADC_MODE_INDEPENDENT;
    HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode);

    /* Calibrate ADC */
    HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);

    /* Configure channels - EMG inputs on PA0, PA1, PA3, PA4 */
    const uint32_t channel_map[ADC_NUM_CHANNELS] = {
        ADC_CHANNEL_0,   /* PA0 - Laryngeal Left (Superior) */
        ADC_CHANNEL_1,   /* PA1 - Laryngeal Right (Superior) */
        ADC_CHANNEL_3,   /* PA3 - Laryngeal Left (Inferior) */
        ADC_CHANNEL_4    /* PA4 - Laryngeal Right (Inferior) */
    };

    for (int i = 0; i < ADC_NUM_CHANNELS; i++) {
        sConfig.Channel = channel_map[i];
        sConfig.Rank = ADC_REGULAR_RANK_1 + i;
        sConfig.SamplingTime = ADC_SAMPLETIME_24CYCLES_5;
        sConfig.SingleDiff = ADC_SINGLE_ENDED;
        sConfig.OffsetNumber = ADC_OFFSET_NONE;
        sConfig.Offset = 0;
        HAL_ADC_ConfigChannel(&hadc1, &sConfig);
    }
}

/**
 * @brief Initialize Timer2 for timing and ADC trigger
 */
static void TIM_Init(void)
{
    __HAL_RCC_TIM2_CLK_ENABLE();

    htim2.Instance = TIM2;
    htim2.Init.Prescaler = (SYSTEM_CLOCK_HZ / ADC_SAMPLE_RATE_HZ) - 1; /* 1kHz */
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 1 - 1;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    HAL_TIM_Base_Init(&htim2);

    /* Configure timer interrupt for 1ms tick */
    HAL_NVIC_SetPriority(TIM2_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

/**
 * @brief Initialize USART2 (ST-Link VCOM)
 */
static void UART_Init(void)
{
    __HAL_RCC_USART2_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* PA2 - USART_TX, PA3 - USART_RX (alternate function) */
    GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Configure USART */
    huart2.Instance = USART2;
    huart2.Init.BaudRate = UART_BAUDRATE;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;

    HAL_UART_Init(&huart2);
}

/* ============================================================================
 * ISR HANDLERS
 * ============================================================================ */

void DMA1_Channel1_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&hdma_adc1);
}

void TIM2_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&htim2);
}

/* DMA Callbacks */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if (hadc->Instance == ADC1) {
        adc_dma_complete = 1;
        HAL_GPIO_TogglePin(STATUS_LED_PORT, STATUS_LED_PIN);
    }
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
{
    if (hadc->Instance == ADC1) {
        adc_half_complete = 1;
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
    if (htim->Instance == TIM2) {
        system_tick_ms++;
    }
}

/* ============================================================================
 * EMG PROCESSING
 * ============================================================================ */

/**
 * @brief Calculate mean of buffer
 */
static uint16_t Calculate_Mean(uint16_t* data, uint32_t len)
{
    uint32_t sum = 0;
    for (uint32_t i = 0; i < len; i++) {
        sum += data[i];
    }
    return (uint16_t)(sum / len);
}

/**
 * @brief Process ADC data to extract EMG channel values
 * Calculates rolling average from DMA buffer
 */
static void Process_EMG_Data(void)
{
    uint32_t offset = adc_half_complete ? (ADC_BUFFER_SIZE / 2) : 0;
    
    /* Process each channel */
    for (int ch = 0; ch < ADC_NUM_CHANNELS; ch++) {
        uint32_t ch_offset = offset + ch;
        uint16_t samples[ADC_SAMPLES_PER_CHANNEL / 2];
        
        /* Extract samples for this channel */
        for (int i = 0; i < (ADC_SAMPLES_PER_CHANNEL / 2); i++) {
            samples[i] = adc_dma_buffer[ch_offset + (i * ADC_NUM_CHANNELS)];
        }
        
        /* Calculate mean value */
        emg_channels[ch] = Calculate_Mean(samples, ADC_SAMPLES_PER_CHANNEL / 2);
    }

    adc_half_complete = 0;
    adc_dma_complete = 0;
}

/**
 * @brief Update active/inactive status for each channel with hysteresis
 */
static void Update_Channel_Status(void)
{
    uint32_t now = GetTick_ms();
    uint8_t any_active = 0;

    for (int ch = 0; ch < ADC_NUM_CHANNELS; ch++) {
        uint16_t value = emg_channels[ch];
        bool was_active = emg_active[ch];
        bool is_active;

        /* Apply hysteresis */
        if (was_active) {
            is_active = (value > (EMG_THRESHOLD_HIGH - EMG_HYSTERESIS));
        } else {
            is_active = (value > EMG_THRESHOLD_HIGH);
        }

        /* Track activation time */
        if (!was_active && is_active) {
            emg_activity_start[ch] = now;
        }

        emg_active[ch] = is_active;
        if (is_active) any_active = 1;
    }

    /* Update status LED */
    HAL_GPIO_WritePin(EMG_ACTIVE_PORT, EMG_ACTIVE_PIN, any_active ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/**
 * @brief Detect command patterns from EMG channel activations
 * 
 * Pattern mapping (4 channels = 16 possible combinations):
 *  CH0 = Laryngeal Left Superior (LLS)
 *  CH1 = Laryngeal Right Superior (LRS)
 *  CH2 = Laryngeal Left Inferior (LLI)
 *  CH3 = Laryngeal Right Inferior (LRI)
 * 
 * Basic command set (currently active commands):
 *  - Single channel = Vowel/consonant sounds
 *  - Double channel combos = Word triggers
 */
static void Detect_Commands(void)
{
    uint32_t now = GetTick_ms();
    uint8_t active_mask = 0;
    uint8_t held_long_enough = 0;

    /* Build activation mask */
    for (int ch = 0; ch < ADC_NUM_CHANNELS; ch++) {
        if (emg_active[ch]) {
            active_mask |= (1 << ch);
            
            /* Check hold time */
            if ((now - emg_activity_start[ch]) >= CMD_HOLD_TIME_MS) {
                held_long_enough |= (1 << ch);
            }
        }
    }

    /* State machine for command detection */
    switch (cmd_state.state) {
        case CMD_STATE_IDLE:
            if (held_long_enough && active_mask) {
                cmd_state.state = CMD_STATE_DETECTING;
                cmd_state.channel_mask = active_mask;
                cmd_state.start_time = now;
            }
            break;

        case CMD_STATE_DETECTING:
            /* Check if same pattern held consistently */
            if (active_mask != cmd_state.channel_mask) {
                /* Pattern changed, reset */
                cmd_state.state = CMD_STATE_IDLE;
            } else if ((now - cmd_state.start_time) >= CMD_HOLD_TIME_MS) {
                /* Convert to command */
                cmd_state.state = CMD_STATE_CONFIRMED;
            }
            break;

        case CMD_STATE_CONFIRMED:
            {
                /* Map pattern to command string */
                const char* cmd = NULL;
                
                switch (cmd_state.channel_mask) {
                    /* Single channel - 4 commands */
                    case 0x01: cmd = "CMD:VOWEL_A"; break;      /* CH0 */
                    case 0x02: cmd = "CMD:VOWEL_E"; break;      /* CH1 */
                    case 0x04: cmd = "CMD:VOWEL_I"; break;      /* CH2 */
                    case 0x08: cmd = "CMD:VOWEL_O"; break;      /* CH3 */
                    
                    /* Double channel - 6 combinations */
                    case 0x03: cmd = "CMD:WORD_YES"; break;     /* CH0+CH1 */
                    case 0x05: cmd = "CMD:WORD_NO"; break;      /* CH0+CH2 */
                    case 0x09: cmd = "CMD:WORD_HELLO"; break;   /* CH0+CH3 */
                    case 0x06: cmd = "CMD:WORD_STOP"; break;    /* CH1+CH2 */
                    case 0x0A: cmd = "CMD:WORD_GO"; break;      /* CH1+CH3 */
                    case 0x0C: cmd = "CMD:WORD_OK"; break;      /* CH2+CH3 */
                    
                    /* Triple/Quad - reserved */
                    case 0x0F: cmd = "CMD:EMERGENCY"; break;    /* All channels */
                    
                    default: cmd = "CMD:UNKNOWN"; break;
                }
                
                if (cmd) {
                    Send_Command(cmd);
                    cmd_state.last_trigger = now;
                }
                
                cmd_state.state = CMD_STATE_COOLDOWN;
            }
            break;

        case CMD_STATE_COOLDOWN:
            /* Wait for release + cooldown period */
            if (!active_mask && (now - cmd_state.last_trigger) >= CMD_COOLDOWN_MS) {
                cmd_state.state = CMD_STATE_IDLE;
            }
            break;
    }
}

/* ============================================================================
 * COMMAND OUTPUT
 * ============================================================================ */

/**
 * @brief Send command via UART with timestamp
 */
static void Send_Command(const char* cmd)
{
    char tx_buf[UART_TX_BUF_SIZE];
    uint32_t timestamp = GetTick_ms();
    
    /* Format: [TIMESTAMP] COMMAND\r\n */
    int len = snprintf(tx_buf, sizeof(tx_buf), "[%08lu] %s\r\n", timestamp, cmd);
    
    if (len > 0 && len < UART_TX_BUF_SIZE) {
        HAL_UART_Transmit(&huart2, (uint8_t*)tx_buf, len, 100);
    }
}

/**
 * @brief Send debug string via UART
 */
static void UART_SendString(const char* str)
{
    HAL_UART_Transmit(&huart2, (uint8_t*)str, strlen(str), 100);
}

/* ============================================================================
 * UTILITIES
 * ============================================================================ */

static uint32_t GetTick_ms(void)
{
    return system_tick_ms;
}

/* ============================================================================
 * MAIN
 * ============================================================================ */

int main(void)
{
    /* Initialize HAL */
    HAL_Init();
    
    /* Configure system clock to 80MHz */
    SystemClock_Config();
    
    /* Initialize all peripherals */
    DMA_Init();
    GPIO_Init();
    ADC_Init();
    UART_Init();
    TIM_Init();
    
    /* Start timer for 1ms tick */
    HAL_TIM_Base_Start_IT(&htim2);
    
    /* Start ADC with DMA in circular mode */
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_dma_buffer, ADC_BUFFER_SIZE);
    
    /* Send boot message */
    UART_SendString("\r\n========================================\r\n");
    UART_SendString("SILENT-001 Firmware v");
    UART_SendString(FIRMWARE_VERSION);
    UART_SendString("\r\nBoot complete. Sampling EMG...\r\n");
    UART_SendString("========================================\r\n");
    
    /* Initialize command state */
    memset(&cmd_state, 0, sizeof(cmd_state));
    
    /* Main loop */
    while (1) {
        /* Process ADC when DMA completes */
        if (adc_dma_complete || adc_half_complete) {
            Process_EMG_Data();
            Update_Channel_Status();
            Detect_Commands();
        }
        
        /* Low power mode when idle */
        __WFI();
    }
}

/* ============================================================================
 * ERROR HANDLERS
 * ============================================================================ */

void Error_Handler(void)
{
    /* Turn on status LED and block */
    HAL_GPIO_WritePin(STATUS_LED_PORT, STATUS_LED_PIN, GPIO_PIN_SET);
    while (1) {
        // Stay here for debugging
    }
}

void assert_failed(uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number */
    Error_Handler();
}