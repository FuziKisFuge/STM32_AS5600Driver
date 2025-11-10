/*
 * AS5600PeripheralInit.c
 *
 *  Created on: Nov 6, 2025
 *      Author: Z440
 */

#include "AS5600Driver.h"


/* TIM2 init function */
void AS5600_TimerInit(AS5600Handle_Typedef *pAS)
{

  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  //pAS->htim->Instance = TIM2;
  pAS->htim->Init.Prescaler = 0;
  pAS->htim->Init.CounterMode = TIM_COUNTERMODE_UP;
  pAS->htim->Init.Period = 4294967295;
  pAS->htim->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  pAS->htim->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_IC_Init(pAS->htim) != HAL_OK)
  {
	  AS5600_Peripherial_ErrorHandler();
  }
  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_RESET;
  sSlaveConfig.InputTrigger = TIM_TS_TI1FP1;
  sSlaveConfig.TriggerPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sSlaveConfig.TriggerPrescaler = TIM_ICPSC_DIV1;
  sSlaveConfig.TriggerFilter = 0;
  if (HAL_TIM_SlaveConfigSynchro(pAS->htim, &sSlaveConfig) != HAL_OK)
  {
	  AS5600_Peripherial_ErrorHandler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(pAS->htim, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
	  AS5600_Peripherial_ErrorHandler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
  sConfigIC.ICSelection = TIM_ICSELECTION_INDIRECTTI;
  if (HAL_TIM_IC_ConfigChannel(pAS->htim, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
  {
	  AS5600_Peripherial_ErrorHandler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(pAS->htim, &sMasterConfig) != HAL_OK)
  {
	  AS5600_Peripherial_ErrorHandler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

__attribute__((weak)) void HAL_TIM_IC_MspInit(TIM_HandleTypeDef* tim_icHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(tim_icHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspInit 0 */

  /* USER CODE END TIM2_MspInit 0 */
    /* TIM2 clock enable */
    __HAL_RCC_TIM2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**TIM2 GPIO Configuration
    PA5     ------> TIM2_CH1
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN TIM2_MspInit 1 */

  /* USER CODE END TIM2_MspInit 1 */
  }
}

__attribute__((weak)) void HAL_TIM_IC_MspDeInit(TIM_HandleTypeDef* tim_icHandle)
{

  if(tim_icHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspDeInit 0 */

  /* USER CODE END TIM2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM2_CLK_DISABLE();

    /**TIM2 GPIO Configuration
    PA5     ------> TIM2_CH1
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5);

  /* USER CODE BEGIN TIM2_MspDeInit 1 */

  /* USER CODE END TIM2_MspDeInit 1 */
  }
}






void AS5600_Peripherial_ErrorHandler()
{

}




























//--------------WEAK FUNCTION------------
__attribute__((weak)) HAL_StatusTypeDef HAL_TIM_IC_Init(TIM_HandleTypeDef *htim)
{
  /* Check the TIM handle allocation */
  if (htim == NULL)
  {
    return HAL_ERROR;
  }

  /* Check the parameters */
  assert_param(IS_TIM_INSTANCE(htim->Instance));
  assert_param(IS_TIM_COUNTER_MODE(htim->Init.CounterMode));
  assert_param(IS_TIM_CLOCKDIVISION_DIV(htim->Init.ClockDivision));
  assert_param(IS_TIM_PERIOD(htim, htim->Init.Period));
  assert_param(IS_TIM_AUTORELOAD_PRELOAD(htim->Init.AutoReloadPreload));

  if (htim->State == HAL_TIM_STATE_RESET)
  {
    /* Allocate lock resource and initialize it */
    htim->Lock = HAL_UNLOCKED;

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
    /* Reset interrupt callbacks to legacy weak callbacks */
    TIM_ResetCallback(htim);

    if (htim->IC_MspInitCallback == NULL)
    {
      htim->IC_MspInitCallback = HAL_TIM_IC_MspInit;
    }
    /* Init the low level hardware : GPIO, CLOCK, NVIC */
    htim->IC_MspInitCallback(htim);
#else
    /* Init the low level hardware : GPIO, CLOCK, NVIC and DMA */
    HAL_TIM_IC_MspInit(htim);
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
  }

  /* Set the TIM state */
  htim->State = HAL_TIM_STATE_BUSY;

  /* Init the base time for the input capture */
  TIM_Base_SetConfig(htim->Instance, &htim->Init);

  /* Initialize the DMA burst operation state */
  htim->DMABurstState = HAL_DMA_BURST_STATE_READY;

  /* Initialize the TIM channels state */
  TIM_CHANNEL_STATE_SET_ALL(htim, HAL_TIM_CHANNEL_STATE_READY);
  TIM_CHANNEL_N_STATE_SET_ALL(htim, HAL_TIM_CHANNEL_STATE_READY);

  /* Initialize the TIM state*/
  htim->State = HAL_TIM_STATE_READY;

  return HAL_OK;
}


__attribute__((weak)) uint32_t HAL_TIM_ReadCapturedValue(const TIM_HandleTypeDef *htim, uint32_t Channel)
{
  uint32_t tmpreg = 0U;

  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      /* Check the parameters */
      assert_param(IS_TIM_CC1_INSTANCE(htim->Instance));

      /* Return the capture 1 value */
      tmpreg =  htim->Instance->CCR1;

      break;
    }
    case TIM_CHANNEL_2:
    {
      /* Check the parameters */
      assert_param(IS_TIM_CC2_INSTANCE(htim->Instance));

      /* Return the capture 2 value */
      tmpreg =   htim->Instance->CCR2;

      break;
    }

    case TIM_CHANNEL_3:
    {
      /* Check the parameters */
      assert_param(IS_TIM_CC3_INSTANCE(htim->Instance));

      /* Return the capture 3 value */
      tmpreg =   htim->Instance->CCR3;

      break;
    }

    case TIM_CHANNEL_4:
    {
      /* Check the parameters */
      assert_param(IS_TIM_CC4_INSTANCE(htim->Instance));

      /* Return the capture 4 value */
      tmpreg =   htim->Instance->CCR4;

      break;
    }

    default:
      break;
  }

  return tmpreg;
}



__attribute__((weak)) HAL_StatusTypeDef HAL_TIM_IC_Start(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  uint32_t tmpsmcr;
  HAL_TIM_ChannelStateTypeDef channel_state = TIM_CHANNEL_STATE_GET(htim, Channel);
  HAL_TIM_ChannelStateTypeDef complementary_channel_state = TIM_CHANNEL_N_STATE_GET(htim, Channel);

  /* Check the parameters */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));

  /* Check the TIM channel state */
  if ((channel_state != HAL_TIM_CHANNEL_STATE_READY)
      || (complementary_channel_state != HAL_TIM_CHANNEL_STATE_READY))
  {
    return HAL_ERROR;
  }

  /* Set the TIM channel state */
  TIM_CHANNEL_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_BUSY);
  TIM_CHANNEL_N_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_BUSY);

  /* Enable the Input Capture channel */
  TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_ENABLE);

  /* Enable the Peripheral, except in trigger mode where enable is automatically done with trigger */
  if (IS_TIM_SLAVE_INSTANCE(htim->Instance))
  {
    tmpsmcr = htim->Instance->SMCR & TIM_SMCR_SMS;
    if (!IS_TIM_SLAVEMODE_TRIGGER_ENABLED(tmpsmcr))
    {
      __HAL_TIM_ENABLE(htim);
    }
  }
  else
  {
    __HAL_TIM_ENABLE(htim);
  }

  /* Return function status */
  return HAL_OK;
}


__attribute__((weak)) void TIM_CCxChannelCmd(TIM_TypeDef *TIMx, uint32_t Channel, uint32_t ChannelState)
{
  uint32_t tmp;

  /* Check the parameters */
  assert_param(IS_TIM_CC1_INSTANCE(TIMx));
  assert_param(IS_TIM_CHANNELS(Channel));

  tmp = TIM_CCER_CC1E << (Channel & 0x1FU); /* 0x1FU = 31 bits max shift */

  /* Reset the CCxE Bit */
  TIMx->CCER &= ~tmp;

  /* Set or reset the CCxE Bit */
  TIMx->CCER |= (uint32_t)(ChannelState << (Channel & 0x1FU)); /* 0x1FU = 31 bits max shift */
}


