# Estimadores-para-DFSA
O objetivo deste projeto é o desenvolvimento de um simulador para a avaliação de desempenho de estimadores DFSA.

Os algoritmos anti colisão implementados foram:
  - __Lower Bound__
  - __Eom Lee__
  
## How to Run
Para rodar o simulador, entre na pasta _src_ e digite o segunte comando:

*make simulation*

Agora o executavel do programa está na pasta _bin_.

Esse programa gera como resultado imagens de gráficos de comparação entre as execuções. Os mesmos se encontram na pasta _plots/results_.


# ENGLISH VERSION

# Dynamic Framed Slotted ALOHA (DFSA) estimator
The goal of this project is the implementation of a simulator for the estimators' performance evaluation.

The anti-colision algorithms implemented:
  - __Lower Bound__
  - __Eom Lee__

## How to Run
to run the simulator, enter the _src_ folder and write the following command:

*make simulation*

Now the executable file is in the _bin_ folder.

This program generates graphs images as result of the comparation between the algorithms. Results are found at the _plots/results_ folder.

## Input example:
_./simulation 100 1000 100 64 2000_

which are: *initial_tag*, *final_tag*, *delta_tag*, *initial_frame_size*, *number_of_repetitions*. 
