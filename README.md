# butterworth_low_pass_filter
 filtro butterworth low pass filter:
 
 % ESPECIFICAÇÕES DO FILTRO
fs = 48000;             % Taxa de amostragem em Hz
Fs_Hz=fs;
stopband_freq = 3000;   % Frequência de corte da banda de rejeição em Hz
passband_freq = 2000;   % Frequência de corte da banda passante em Hz

delta_passband = 0.01;  % Tolerância de ganho na banda passante
delta_stopband = 0.001; % Tolerância de ganho na banda de rejeição
 

## Descrição

Projeto de analise de desempenho do filtro passa baixa butterworth , para comparação da implementação  do filtro utilizando sistemas de transformação direta do filtro comparando com a implementação em cascata de segunda ordem ( BIQUADS ) usando a forma direta II
em um microcontrolador baseado em Cortex-M com aplicações de ponto flutuante e ponto fixo por meio da estrutura de software Arm CMSIS-DSP.


## ❓ Como usar

### Ferramentas Necessárias

- **[STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html)** 6.7.0 - Necessário para abrir o arquivo `butterworth_low_pass_filter.ioc` e gerar as bibliotecas de código.

- **[STM32CubeProgrammer](https://www.st.com/en/development-tools/stm32cubeprog.html)** v2.9.0 - Necessário para gravar o microcontrolador.


- **[STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html)** v1.12.0 - IDE que gera o ambiente de compilação do projeto. Engloba o STM32CubeProgrammer e o STM32CubeMX, não sendo necessário instalá-los separadamente.

    - Caso o projeto seja importado para o STM32CubeIDE, ao abrir o arquivo `butterworth_low_pass_filter.ioc`, será mostrado um prompt para a instalação do pacote automaticamente.
    
- **[MATLAB](https://matlab.mathworks.com/)**  o projeto do matlab  gera os coeficientes do filtro no formato da linguagem C compativel com a implementação no microcontrolado, com ele tambem é possivel fazer simulações do filtro "Matlab_gerador_coeficientes.m"

### Clonar o Repositório

Utilizando um terminal ou algum cliente git a sua escolha, clonar o repositório do projeto:

```
git clone git@github.com:User/butterworth_low_pass_filter.git
```

### Gerar o Código e Compilar o Projeto

Abra o STM32CubeIDE e importe o projeto utilizando a opção `File -> Import... -> General -> Existing Project into Workspace`. Busque a pasta do projeto clonado, selecione-a e clique em `Finish`.

Com o projeto importado, abra o arquivo `butterworth_low_pass_filter.ioc`. Após isso, selecione a opção `Project -> Generate Code` (Alt + K) para gerar as bibliotecas necessárias.

clique com o botão direito no projeto e `Build Project`.

### Gravar o Microcontrolador

Após compilar o projeto, com um gravador conectado ao microcontrolador, use a opção `Run` nas configurações padrão para gravá-lo.

## 📂 Estrutura de Pastas

- **Core/**

    - Arquivos fonte .c/.h relacionadas ao firmware .
    
    - Controle de periféricos, inicialização da main
    
    - Drivers/microfone driver  .

- **Drivers/**

	-DPS arquivos da biblioteca Arm CMSIS-DSP
    - Aqui serão geradas as libs HAL da ST pelo CubeMX .

## 😉 Arquivos Relevantes 

- **Core/Src/low_pass_filter.c** arquivo de definição dos coeficientes do filtro butterworth.
