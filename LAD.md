
#### Para conectar no lad

ssh portoalegre\\zurawski.gabriel@sparta.pucrs.br

ssh cp14@atlantica.lad.pucrs.br

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Copiar arquivos via SCP estando na rede da PUCRS:
Copiar um arquivo da máquina local para a Grad:

scp arquivo.txt cpxx@atlantica.lad.pucrs.br:/home/cpxx/
Copiar um arquivo da Grad para a máquina local:

scp cpxx@atlantica.lad.pucrs.br:/home/cpxx/arquivo.txt ./

Copiar arquivos via SCP estando fora da rede da PUCRS:

A máquina Atlantica só pode ser acessa diretamente de dentro da rede da PUCRS. O acesso externo só é possível através da máquina Sparta. A cópia de arquivos pode tanto ser feita a partir da Sparta quanto da Atlantica.

Copiar um arquivo da máquina local para a Sparta:

scp arquivo.txt <usuario e-mail>@sparta.pucrs.br:~/

Copiar um arquivo da Sparta para a Atlantica (estando na Sparta):

scp arquivo.txt cpxx@atlantica.lad.pucrs.br:/home/cpxx

Copiar um arquivo da Sparta para a Atlantica (estando na Atlantica):

scp <usuario e-mail>@sparta.pucrs.br:./arquivo.txt ./

Copiar um arquivo da Grad para a Sparta (estando na Sparta):

scp cpxx@atlantica.lad.pucrs.br:/home/cpxx/arquivo.txt ./