# Agenda Telefônica

Baseado nos conceitos de C sobre Ponteiros, Structs e Arquivos, desenvolva um sistema para agenda telefônica, o qual deve ser capaz de gravar os dados das pessoas em um arquivo.

Os dados a serem gravados são: Código de registro, Nome da pessoa, Número do telefone, Endereço (composto por Logradouro, Número, Bairro, CEP, Cidade, Estado, País).

De acordo com as requisições do cliente, o sistema deve permitir as seguintes funcionalidades, através da manipulação dos registros do arquivo:

- Adicionar um novo contato;
- Remover um contato já existente (se não existe informar);
- Visualizar a agenda (todos os dados já cadastrados);
- Fazer a busca de um contato (através do Código do registro e/ou do Nome), permitindo visualizar todos os dados do contato;
- Editar um contato, através da busca do contato (Código do registro e/ou do Nome);

O programa deve criar a agenda e, uma vez criado, este arquivo não pode ser apagado. Assim,
deve-se permitir apenas a manipulação deste arquivo, de modo que os dados já gravados possam ser editados, inseridos novos dados ou apagados de acordo com a busca dos dados.

Requisitos para Implementação:

- Use uma struct para representar os contatos;
- A agenda tem tamanho total de 100 registros;
- O único dado que não pode ser repetido é o Código de Registro;
- Implemente, pelo menos, os seguintes protótipos para as funcionalidades requeridas.

Lembre-se que parâmetros podem ser passados para as funções, assim como pode-se ter retorno das funções:

- adicionar_contato();
- remover_contato();
- visualizar_contato();
- buscar_contato();
- editar_contato();
- carregar_arquivo();
- salvar_arquivo()