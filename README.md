# Trabalho 2 de Estrutura de Dados - Order Book

## Descrição do Projeto
Este repositório é destinado ao trabalho 2 da disciplina de Estrutura de Dados, que consiste na implementação de um Livro de Ofertas (*Order Book*) financeiro. O sistema é responsável por receber ordens de compra (`B` - *Buy*) e de venda (`S` - *Sell*) e realizar o cruzamento dessas ordens (match) com base em regras de prioridade de Preço e Tempo. Além de submeter e cancelar ordens, o sistema registra as transações executadas com sucesso.

## Instruções de Compilação
Abra o terminal no diretório raiz do projeto e execute o seguinte comando:

```bash
g++ main.cpp OrderBook.cpp Order.cpp Transaction.cpp -o order_book
```

## Instruções de Execução
Após compilar o projeto com sucesso, inicie o executável gerado através do terminal:

```Bash
./order_book
```
A execução rodará a bateria de testes definida no main.cpp, exibindo no terminal o estado do livro de ofertas (compras e vendas pendentes) e as transações executadas em diferentes exemplos (ordens sem execução, execução imediata, cancelamentos, etc).

## Organização Interna dos Dados
Para garantir um processamento rápido e aplicar os conceitos de listas encadeadas, o sistema foi organizado da seguinte maneira:

### Livro de Ordens (Order Book)
Ambas as ordens de compra e de venda ficam armazenadas na mesma lista duplamente encadeada e circular, utilizando um nó sentinela (Head fictício, chamado H).

Topologia da Lista: A lista se estrutura no formato ... -> S3 -> S2 -> S1 -> H -> B1 -> B2 -> B3 -> ... (onde Sn são ordens de venda e Bn de compra).

Prioridade Temporal: Ordens inseridas primeiro (menor timestamp) ficam sempre mais próximas ao nó H para manter a ordenação histórica.

Eficiência: A escolha por uma lista circular permite que, a partir da Head, seja possível acessar rapidamente tanto a ponta das ordens de compra (head->next) quanto a ponta das ordens de venda (head->prev), além de facilitar a inserção nas extremidades em tempo constante O(1) quando uma ordem não encontra contraparte e vai descansar no livro.

### Histórico de Transações
O registro das transações executadas (matches) é isolado em uma lista simplesmente encadeada.

Para garantir performance máxima durante a execução das ordens, esta lista mantém os ponteiros transactionsHead (início) e transactionsTail (fim). Isso possibilita a inserção de novos registros de transações ao final da lista em complexidade de tempo O(1).
