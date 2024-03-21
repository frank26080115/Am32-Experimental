/*
 * peripherals.c
 *
 *  Created on: Sep. 26, 2020
 *      Author: Alka
 */

// PERIPHERAL SETUP

#define KR_KEY_Reload ((uint16_t)0xAAAA)
#define KR_KEY_Enable ((uint16_t)0xCCCC)

#include "peripherals.h"

#include "ADC.h"
#include "common.h"
#include "functions.h"
#include "serial_telemetry.h"
#include "targets.h"

void initCorePeripherals(void)
{
    system_clock_config();
    // LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);
    // LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

    // fmc_wscnt_set(2);
    // fmc_prefetch_enable();
    MX_GPIO_Init();
    MX_DMA_Init();
    TIM1_Init();
    TIM4_Init();
    TIM9_Init();
    AT_COMP_Init();
    TIM11_Init();
    TIM10_Init();

    UN_TIM_Init();
#ifdef USE_SERIAL_TELEMETRY
    telem_UART_Init();
#endif
}

// COMP_InitType  COMP_InitStructure;
// EXTI_InitType EXTI_InitStructure;

void initAfterJump(void)
{
    __enable_irq();
}

void system_clock_config(void)
{
    //  flash_psr_set(FLASH_WAIT_CYCLE_3);
    //  crm_reset();
    //  crm_clock_source_enable(CRM_CLOCK_SOURCE_HICK, TRUE);
    //  while(crm_flag_get(CRM_HICK_STABLE_FLAG) != SET)
    //  {
    //  }
    //  crm_pll_config(CRM_PLL_SOURCE_HICK, CRM_PLL_MULT_30);
    //  crm_clock_source_enable(CRM_CLOCK_SOURCE_PLL, TRUE);
    //  while(crm_flag_get(CRM_PLL_STABLE_FLAG) != SET)
    //  {
    //  }
    //  crm_ahb_div_set(CRM_AHB_DIV_1);
    //  crm_apb2_div_set(CRM_APB2_DIV_1);
    //  crm_apb1_div_set(CRM_APB1_DIV_1);
    //	crm_auto_step_mode_enable(TRUE);
    //  crm_sysclk_switch(CRM_SCLK_PLL);
    //  while(crm_sysclk_switch_status_get() != CRM_SCLK_PLL)
    //  {
    //  }
    //  crm_auto_step_mode_enable(FALSE);
    //  system_core_clock_update();
    /* config flash psr register */

    /// 144 mhz hick setup

    flash_psr_set(FLASH_WAIT_CYCLE_4);
    crm_reset();

    crm_clock_source_enable(CRM_CLOCK_SOURCE_HICK, TRUE);
    while (crm_flag_get(CRM_HICK_STABLE_FLAG) != SET) {
    }
    crm_pll_config(CRM_PLL_SOURCE_HICK, CRM_PLL_MULT_36);
    crm_clock_source_enable(CRM_CLOCK_SOURCE_PLL, TRUE);
    while (crm_flag_get(CRM_PLL_STABLE_FLAG) != SET) {
    }
    crm_ahb_div_set(CRM_AHB_DIV_1);
    crm_apb2_div_set(CRM_APB2_DIV_2);
    crm_apb1_div_set(CRM_APB1_DIV_2);
    crm_auto_step_mode_enable(TRUE);
    crm_sysclk_switch(CRM_SCLK_PLL);
    while (crm_sysclk_switch_status_get() != CRM_SCLK_PLL) {
    }
    crm_auto_step_mode_enable(FALSE);
    system_core_clock_update();
}

void AT_COMP_Init(void)
{
    //	COMP_StructInit(&COMP_InitStructure);

    crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_CMP_PERIPH_CLOCK, TRUE);

    // rcu_periph_clock_enable(RCU_GPIOA);

    /* configure PA1 as comparator input */
    gpio_mode_QUICK(GPIOA, GPIO_MODE_ANALOG, GPIO_PULL_NONE, GPIO_PINS_1);
    //  rcu_periph_clock_enable(RCU_CFGCMP);

    gpio_mode_QUICK(GPIOA, GPIO_MODE_ANALOG, GPIO_PULL_NONE, GPIO_PINS_5);
    /* configure comparator channel0 */
    //  cmp_mode_init(CMP_HIGHSPEED, CMP_PA5, CMP_HYSTERESIS_NO);

    //		COMP_InitStructure.COMP_INMInput = COMP_INMInput_IN3;
    //		COMP_InitStructure.COMP_Output = COMP_Output_None;
    //		COMP_InitStructure.COMP_OutPolarity =
    // COMP_OutPolarity_NonInverted;
    // COMP_InitStructure.COMP_Hysteresis = COMP_Hysteresis_No;
    // COMP_InitStructure.COMP_Mode = COMP_Mode_Fast;
    //	  COMP_Init(COMP1_Selection, &COMP_InitStructure);
    //  cmp_enable();
    //  delay_1ms(1);

    //   cmp_output_init(CMP_OUTPUT_NONE, CMP_OUTPUT_POLARITY_NOINVERTED);

    /* initialize exti line21 */
    //		EXTI_StructInit(&EXTI_InitStructure);
    //	EXTI_InitStructure.EXTI_Line = EXTI_LINE;
    //  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    //  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    //  EXTI_InitStructure.EXTI_LineEnable = ENABLE;
    //		EXTI_Init(&EXTI_InitStructure);
    //		EXTI_ClearFlag(EXTI_LINE);
    /* configure ADC_CMP NVIC */
    //   nvic_irq_enable(ADC_COMP_IRQHandler, 0);
    //		  NVIC_InitType NVIC_InitStructure;

    //  NVIC_InitStructure.NVIC_IRQChannel = ADC_COMP_IRQn;
    //  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    //  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    //  NVIC_Init(&NVIC_InitStructure);

    NVIC_SetPriority(CMP1_IRQn, 0);
    NVIC_EnableIRQ(CMP1_IRQn);

    //	COMP_Cmd(COMP1_Selection, ENABLE);
    cmp_enable(CMP1_SELECTION, TRUE);
}

void MX_IWDG_Init(void)
{
    //		fwdgt_config(4000,FWDGT_PSC_DIV16);
    //    fwdgt_enable();

    WDT->cmd = WDT_CMD_UNLOCK;
    WDT->cmd = WDT_CMD_ENABLE;
    WDT->div = WDT_CLK_DIV_64;
    WDT->rld = 4000;
    WDT->cmd = WDT_CMD_RELOAD;
}

void TIM1_Init(void)
{
    crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_TMR1_PERIPH_CLOCK, TRUE);

    gpio_pin_remap_config(TMR1_GMUX_0001, TRUE);

    TMR1->pr = 3000;
    TMR1->div = 0;

    TMR1->cm1 = 0x6868; // Channel 1 and 2 in PWM output mode
    TMR1->cm2 = 0x68; // channel 3 in PWM output mode

    tmr_output_channel_buffer_enable(TMR1, TMR_SELECT_CHANNEL_1, TRUE);
    tmr_output_channel_buffer_enable(TMR1, TMR_SELECT_CHANNEL_2, TRUE);
    tmr_output_channel_buffer_enable(TMR1, TMR_SELECT_CHANNEL_3, TRUE);

    tmr_period_buffer_enable(TMR1, TRUE);

    TMR1->brk_bit.dtc = DEAD_TIME;
    crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);

    /*configure PA8/PA9/PA10(TIMER0/CH0/CH1/CH2) as alternate function*/
    gpio_mode_QUICK(PHASE_A_GPIO_PORT_LOW, GPIO_MODE_MUX, GPIO_PULL_NONE,
        PHASE_A_GPIO_LOW);

    gpio_mode_QUICK(PHASE_B_GPIO_PORT_LOW, GPIO_MODE_MUX, GPIO_PULL_NONE,
        PHASE_B_GPIO_LOW);

    gpio_mode_QUICK(PHASE_C_GPIO_PORT_LOW, GPIO_MODE_MUX, GPIO_PULL_NONE,
        PHASE_C_GPIO_LOW);

    /*configure PB13/PB14/PB15(TIMER0/CH0N/CH1N/CH2N) as alternate function*/
    gpio_mode_QUICK(PHASE_A_GPIO_PORT_HIGH, GPIO_MODE_MUX, GPIO_PULL_NONE,
        PHASE_A_GPIO_HIGH);

    gpio_mode_QUICK(PHASE_B_GPIO_PORT_HIGH, GPIO_MODE_MUX, GPIO_PULL_NONE,
        PHASE_B_GPIO_HIGH);

    gpio_mode_QUICK(PHASE_C_GPIO_PORT_HIGH, GPIO_MODE_MUX, GPIO_PULL_NONE,
        PHASE_C_GPIO_HIGH);
}

void TIM4_Init(void)
{
    crm_periph_clock_enable(CRM_TMR4_PERIPH_CLOCK, TRUE);
    TMR4->pr = 0xFFFF;
    TMR4->div = 74;
}

void TIM9_Init(void)
{
    crm_periph_clock_enable(CRM_TMR9_PERIPH_CLOCK, TRUE);
    TMR9->pr = 1000000 / LOOP_FREQUENCY_HZ;
    TMR9->div = 143;
    NVIC_SetPriority(TMR1_BRK_TMR9_IRQn, 3);
    NVIC_EnableIRQ(TMR1_BRK_TMR9_IRQn);
}

void TIM11_Init(void)
{
    crm_periph_clock_enable(CRM_TMR11_PERIPH_CLOCK, TRUE);
    TMR11->pr = 500;
    TMR11->div = 75;
    TMR11->ctrl1_bit.prben = TRUE;

    NVIC_SetPriority(TMR1_TRG_HALL_TMR11_IRQn, 0);
    NVIC_EnableIRQ(TMR1_TRG_HALL_TMR11_IRQn);
}

void TIM10_Init(void)
{
    crm_periph_clock_enable(CRM_TMR10_PERIPH_CLOCK, TRUE);
    TMR10->pr = 0xFFFF;
    TMR10->div = 75;
    TMR10->ctrl1_bit.prben = TRUE;
}

void MX_DMA_Init(void)
{
    crm_periph_clock_enable(CRM_DMA1_PERIPH_CLOCK, TRUE);
    NVIC_SetPriority(DMA1_Channel6_IRQn, 1);
    NVIC_EnableIRQ(DMA1_Channel6_IRQn);
}

void MX_GPIO_Init(void)
{
    //  /* GPIO Ports Clock Enable */
    //  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
    //  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

    //  /**/
    //  LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_15);

    //  /**/
    //  #ifdef USE_ALKAS_DEBUG_LED
    //    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    //    GPIO_InitStruct.Pin = LL_GPIO_PIN_15;
    //    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    //    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    //    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    //    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    //    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    //  #endif
}

void UN_TIM_Init(void)
{
#ifdef USE_TIMER_3_CHANNEL_1
    //	RCC_AHBPeriphClockCmd(RCC_AHBPERIPH_GPIOB,ENABLE);
    //	RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_TMR3, ENABLE);
    crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_TMR3_PERIPH_CLOCK, TRUE);
#endif
#ifdef USE_TIMER_15_CHANNEL_1
    //	RCC_AHBPeriphClockCmd(RCC_AHBPERIPH_GPIOA,ENABLE);
    //	RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_TMR15, ENABLE);
    crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_TMR15_PERIPH_CLOCK, TRUE);
#endif

    crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);

    // GPIO_PinsRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    gpio_pin_remap_config(SWJTAG_MUX_010, TRUE);
    gpio_pin_remap_config(TMR3_MUX_10, TRUE);

    // GPIO_PinsRemapConfig(GPIO_PartialRemap_TMR3, ENABLE);

    //	RCC_AHBPeriphClockCmd(RCC_AHBPERIPH_DMA1,ENABLE);

    crm_periph_clock_enable(CRM_DMA1_PERIPH_CLOCK, TRUE);
    gpio_mode_QUICK(INPUT_PIN_PORT, GPIO_MODE_INPUT, GPIO_PULL_NONE, INPUT_PIN);

    // GPIO_PinAFConfig(INPUT_PIN_PORT, GPIO_PinsSource4, GPIO_AF_1);
    //	gpio_pin_mux_config(INPUT_PIN_PORT, INPUT_PIN, GPIO_MUX_1);

    //	 dma_periph_address_config(INPUT_DMA_CHANNEL,
    //(uint32_t)&TIMER_CH0CV(IC_TIMER_REGISTER));
    //   dma_memory_address_config(INPUT_DMA_CHANNEL, (uint32_t)&dma_buffer);
    //	 INPUT_DMA_CHANNEL->CPBA = (uint32_t)&IC_TIMER_REGISTER->CC1;
    //	 INPUT_DMA_CHANNEL->CMBA = (uint32_t)&dma_buffer;
    //   INPUT_DMA_CHANNEL->CHCTRL |= DMA_DIR_PERIPHERALSRC;

    //   DMA_Reset(INPUT_DMA_CHANNEL);
    //  DMA_DefaultInitParaConfig(&DMA_InitStructure);

    //  DMA_InitStructure.DMA_PeripheralBaseAddr =
    //  (uint32_t)&IC_TIMER_REGISTER->CC1; DMA_InitStructure.DMA_MemoryBaseAddr =
    //  (uint32_t)&dma_buffer; DMA_InitStructure.DMA_Direction =
    //  DMA_DIR_PERIPHERALSRC; DMA_InitStructure.DMA_BufferSize = 32;
    //  DMA_InitStructure.DMA_PeripheralInc = DMA_PERIPHERALINC_DISABLE;
    //  DMA_InitStructure.DMA_MemoryInc = DMA_MEMORYINC_ENABLE;
    //  DMA_InitStructure.DMA_PeripheralDataWidth =
    //  DMA_PERIPHERALDATAWIDTH_HALFWORD; DMA_InitStructure.DMA_MemoryDataWidth =
    //  DMA_MEMORYDATAWIDTH_WORD; DMA_InitStructure.DMA_Mode = DMA_MODE_NORMAL;
    //  DMA_InitStructure.DMA_Priority = DMA_PRIORITY_LOW;
    //  DMA_InitStructure.DMA_MTOM = DMA_MEMTOMEM_DISABLE;
    //  DMA_Init(INPUT_DMA_CHANNEL, &DMA_InitStructure);

    INPUT_DMA_CHANNEL->ctrl = 0X98a; //  PERIPHERAL HALF WORD, MEMROY WORD ,
                                     //  MEMORY INC ENABLE , TC AND ERROR INTS

    NVIC_SetPriority(IC_DMA_IRQ_NAME, 1);
    NVIC_EnableIRQ(IC_DMA_IRQ_NAME);

    IC_TIMER_REGISTER->pr = 0xFFFF;
    IC_TIMER_REGISTER->div = 16;

    // TMR_ARPreloadConfig(IC_TIMER_REGISTER, ENABLE);
    IC_TIMER_REGISTER->ctrl1_bit.prben = TRUE;

    //	NVIC_SetPriority(TMR3_GLOBAL_IRQn, 0);
    //  NVIC_EnableIRQ(TMR3_GLOBAL_IRQn);
    gpio_mode_QUICK(GPIOB, GPIO_MODE_INPUT, GPIO_PULL_NONE, INPUT_PIN);
    // TMR_Cmd(IC_TIMER_REGISTER, ENABLE);
    IC_TIMER_REGISTER->ctrl1_bit.tmren = TRUE;
}

#ifdef USE_RGB_LED // has 3 color led
void LED_GPIO_init()
{
    LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };

    /* GPIO Ports Clock Enable */
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

    LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_8);
    LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_5);
    LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_3);

    GPIO_InitStruct.Pin = LL_GPIO_PIN_8;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LL_GPIO_PIN_5;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LL_GPIO_PIN_3;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

#endif

void reloadWatchDogCounter()
{
    WDT->cmd = WDT_CMD_RELOAD;
}

void disableComTimerInt()
{
    COM_TIMER->iden &= ~TMR_OVF_INT;
}

void enableComTimerInt()
{
    COM_TIMER->iden |= TMR_OVF_INT;
}

void setAndEnableComInt(uint16_t time)
{
    COM_TIMER->cval = 0;
    COM_TIMER->pr = time;
    COM_TIMER->ists = 0x00;
    COM_TIMER->iden |= TMR_OVF_INT;
}

uint16_t
getintervaTimerCount()
{
    return INTERVAL_TIMER->cval;
}

void setintervaTimerCount(uint16_t intertime)
{
    INTERVAL_TIMER->cval = intertime;
}

void setPrescalerPWM(uint16_t presc)
{
    TMR1->div = presc;
}

void setAutoReloadPWM(uint16_t relval)
{
    TMR1->pr = relval;
}

void setDutyCycleAll(uint16_t newdc)
{
    TMR1->c1dt = newdc;
    TMR1->c2dt = newdc;
    TMR1->c3dt = newdc;
}

void setPWMCompare1(uint16_t compareone)
{
    TMR1->c1dt = compareone;
}
void setPWMCompare2(uint16_t comparetwo)
{
    TMR1->c2dt = comparetwo;
}
void setPWMCompare3(uint16_t comparethree)
{
    TMR1->c3dt = comparethree;
}

void generatePwmTimerEvent()
{
    TMR1->swevt |= TMR_OVERFLOW_SWTRIG;
    ;
}

void resetInputCaptureTimer()
{
    IC_TIMER_REGISTER->pr = 0;
    IC_TIMER_REGISTER->cval = 0;
}

void enableCorePeripherals()
{
    tmr_channel_enable(TMR1, TMR_SELECT_CHANNEL_1, TRUE);
    tmr_channel_enable(TMR1, TMR_SELECT_CHANNEL_2, TRUE);
    tmr_channel_enable(TMR1, TMR_SELECT_CHANNEL_3, TRUE);
    tmr_channel_enable(TMR1, TMR_SELECT_CHANNEL_1C, TRUE);
    tmr_channel_enable(TMR1, TMR_SELECT_CHANNEL_2C, TRUE);
    tmr_channel_enable(TMR1, TMR_SELECT_CHANNEL_3C, TRUE);

    TMR1->ctrl1_bit.tmren = TRUE;
    TMR1->brk_bit.oen = TRUE;
    TMR1->swevt |= TMR_OVERFLOW_SWTRIG;
#ifdef USE_RGB_LED
    LED_GPIO_init();
    GPIOB->scr = LL_GPIO_PIN_8; // turn on red
    GPIOB->clr = LL_GPIO_PIN_5;
    GPIOB->clr = LL_GPIO_PIN_3; //
#endif

#ifndef BRUSHED_MODE
    COM_TIMER->ctrl1_bit.tmren = TRUE;
    COM_TIMER->swevt |= TMR_OVERFLOW_SWTRIG;
    COM_TIMER->iden &= ~TMR_OVF_INT;
#endif
    UTILITY_TIMER->ctrl1_bit.tmren = TRUE;
    INTERVAL_TIMER->ctrl1_bit.tmren = TRUE;
    INTERVAL_TIMER->swevt |= TMR_OVERFLOW_SWTRIG;
    TEN_KHZ_TIMER->ctrl1_bit.tmren = TRUE;
    TEN_KHZ_TIMER->swevt |= TMR_OVERFLOW_SWTRIG;
    TEN_KHZ_TIMER->iden |= TMR_OVF_INT;

#ifdef USE_ADC
    ADC_Init();
#endif

#ifdef USE_ADC_INPUT

#else
    tmr_channel_enable(IC_TIMER_REGISTER, IC_TIMER_CHANNEL, TRUE);
    IC_TIMER_REGISTER->ctrl1_bit.tmren = TRUE;
#endif

    NVIC_SetPriority(EXINT15_10_IRQn, 2);
    NVIC_EnableIRQ(EXINT15_10_IRQn);
    EXINT->inten |= EXINT_LINE_15;
}

void crsf_UART_Init(void)
{
    gpio_init_type gpio_init_struct;

    crm_periph_clock_enable(CRM_USART1_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_DMA1_PERIPH_CLOCK, TRUE);

    /* configure the usart2 tx pin */
    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
    gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
    gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
    gpio_init_struct.gpio_pins = GPIO_PINS_6;
    gpio_init_struct.gpio_pull = GPIO_PULL_UP;
    gpio_init(GPIOB, &gpio_init_struct);
//    gpio_pin_mux_config(GPIOB, GPIO_PINS_SOURCE6, GPIO_MUX_0);

    dma_reset(DMA1_CHANNEL5);

    dma_init_type dma_init_struct;
    dma_default_para_init(&dma_init_struct);
    dma_init_struct.buffer_size = 64;
    dma_init_struct.direction = DMA_DIR_PERIPHERAL_TO_MEMORY;
    dma_init_struct.memory_base_addr = (uint32_t)&crsf_buffer;
    dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_BYTE;
    dma_init_struct.memory_inc_enable = TRUE;
    dma_init_struct.peripheral_base_addr = (uint32_t)&USART1->dt;
    dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_BYTE;
    dma_init_struct.peripheral_inc_enable = FALSE;
    dma_init_struct.priority = DMA_PRIORITY_LOW;
    dma_init_struct.loop_mode_enable = TRUE;
    dma_init(DMA1_CHANNEL5, &dma_init_struct);

    DMA1_CHANNEL5->ctrl |= DMA_FDT_INT;
    DMA1_CHANNEL5->ctrl |= DMA_DTERR_INT;

    /* configure usart1 param */
    usart_init(USART1, 42000, USART_DATA_8BITS, USART_STOP_1_BIT);
    usart_transmitter_enable(USART1, TRUE);
    usart_receiver_enable(USART1, TRUE);
    usart_single_line_halfduplex_select(USART1, TRUE);
    usart_dma_receiver_enable(USART1, TRUE);
    usart_enable(USART1, TRUE);

    nvic_irq_enable(DMA1_Channel5_IRQn, 3, 0);
}

void receiveCRSF(){
	  DMA1_CHANNEL5->dtcnt = 64;
    DMA1_CHANNEL5->ctrl_bit.chen = TRUE;
}

