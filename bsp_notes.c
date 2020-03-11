{ 127, 191, 255, 191, 127, 63, 0, 63, 127, 191, 255, 191, 127, 63, 0, 63, 255, 255, 255, 255, 0, 0, 0, 0, 255, 255, 255, 255, 0, 0, 0, 0 }



typedef struct
{
  uint32_t Channel;              // Specifies the channel used for the specified stream. 
  uint32_t Direction;            // Specifies if the data will be transferred from memory to peripheral, 
  uint32_t PeriphInc;            // Specifies whether the Peripheral address register should be incremented or not.
  uint32_t MemInc;               // Specifies whether the memory address register should be incremented or not.
  uint32_t PeriphDataAlignment;  // Specifies the Peripheral data width.
  uint32_t MemDataAlignment;     // Specifies the Memory data width.
  uint32_t Mode;                 // Specifies the operation mode of the DMAy Streamx.
  uint32_t Priority;             // Specifies the software priority for the DMAy Streamx.
  uint32_t FIFOMode;             // Specifies if the FIFO mode or Direct mode will be used for the specified stream.
  uint32_t FIFOThreshold;        // Specifies the FIFO threshold level.
  uint32_t MemBurst;             // Specifies the Burst transfer configuration for the memory transfers. 
  uint32_t PeriphBurst;          // Specifies the Burst transfer configuration for the peripheral transfers. 
}DMA_InitTypeDef;


typedef struct
{
  DAC_TypeDef                 *Instance;     /*!< Register base address             */
  __IO HAL_DAC_StateTypeDef   State;         /*!< DAC communication state           */
  HAL_LockTypeDef             Lock;          /*!< DAC locking object                */
  DMA_HandleTypeDef           *DMA_Handle1;  /*!< Pointer DMA handler for channel 1 */
  DMA_HandleTypeDef           *DMA_Handle2;  /*!< Pointer DMA handler for channel 2 */
  __IO uint32_t               ErrorCode;     /*!< DAC Error code                    */
}DAC_HandleTypeDef;


typedef struct __DMA_HandleTypeDef
{
  DMA_Stream_TypeDef         *Instance;                                                        /*!< Register base address                  */
  DMA_InitTypeDef            Init;                                                             /*!< DMA communication parameters           */ 
  HAL_LockTypeDef            Lock;                                                             /*!< DMA locking object                     */  
  __IO HAL_DMA_StateTypeDef  State;                                                            /*!< DMA transfer state                     */
  void                       *Parent;                                                          /*!< Parent object state                    */ 
  void                       (* XferCpltCallback)( struct __DMA_HandleTypeDef * hdma);         /*!< DMA transfer complete callback         */
  void                       (* XferHalfCpltCallback)( struct __DMA_HandleTypeDef * hdma);     /*!< DMA Half transfer complete callback    */
  void                       (* XferM1CpltCallback)( struct __DMA_HandleTypeDef * hdma);       /*!< DMA transfer complete Memory1 callback */
  void                       (* XferM1HalfCpltCallback)( struct __DMA_HandleTypeDef * hdma);   /*!< DMA transfer Half complete Memory1 callback */
  void                       (* XferErrorCallback)( struct __DMA_HandleTypeDef * hdma);        /*!< DMA transfer error callback            */
  void                       (* XferAbortCallback)( struct __DMA_HandleTypeDef * hdma);        /*!< DMA transfer Abort callback            */  
  __IO uint32_t              ErrorCode;                                                        /*!< DMA Error code                          */
  uint32_t                   StreamBaseAddress;                                                /*!< DMA Stream Base Address                */
  uint32_t                   StreamIndex;                                                      /*!< DMA Stream Index                       */
}DMA_HandleTypeDef;


typedef struct
{
  __IO uint32_t CR;     /*!< DMA stream x configuration register      */
  __IO uint32_t NDTR;   /*!< DMA stream x number of data register     */
  __IO uint32_t PAR;    /*!< DMA stream x peripheral address register */
  __IO uint32_t M0AR;   /*!< DMA stream x memory 0 address register   */
  __IO uint32_t M1AR;   /*!< DMA stream x memory 1 address register   */
  __IO uint32_t FCR;    /*!< DMA stream x FIFO control register       */
} DMA_Stream_TypeDef;