#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <limits>
#include <locale>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <filesystem>

using namespace std;

#define MAX 100

// Classe de Funções Auxiliares
class Utils {
public:
    static string limparCPF(const string& cpf);
    static string formatarCPF(const string& cpf);

    static string lerString(const string& prompt);
    static int lerOpc();

    static void limparBuffer();
    static void imprimirLinha(char ch = '-', int tamanho = 50);
    static void pausar();

    static void limparTela();
};

// Funções Relacionadas a CPF (Auxiliar)
string Utils::limparCPF(const string& cpf) {
    string limpo;

    for (char c : cpf) {
        if (isdigit(c)) {
            limpo += c;
        }
    }

    return limpo;
}

string Utils::formatarCPF(const string& cpf) {
    string indentado;
    string cpfNumeros = limparCPF(cpf);

    if (cpfNumeros.length() == 11) {
        indentado = cpfNumeros.substr(0, 3) + "." +
                    cpfNumeros.substr(3, 3) + "." +
                    cpfNumeros.substr(6, 3) + "-" +
                    cpfNumeros.substr(9, 2);
    } else {
        indentado = "";
    }

    return indentado;
}

// Funções Relacionada a Leitura (Auxiliar)
int Utils::lerOpc() {
    int opcao;

    while (!(cin >> opcao)) {
        cout << "Opção inválida! Digite um número: ";
        cin.clear();
        limparBuffer();
    }

    limparBuffer();
    return opcao;
}

string Utils::lerString(const string& prompt) {
    string resposta;

    cout << prompt;
    getline(cin, resposta);

    return resposta;
}

// Funções de Extras de Execução (Auxiliar)
void Utils::limparBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void Utils::imprimirLinha(char ch, int tamanho) {
    cout << string(tamanho, ch) << endl;
}

void Utils::pausar() {
    cout << "Pressione Enter para Continuar..." << endl;
    cin.get();
}

void Utils::limparTela() {
    for (int i = 0; i < 50; ++i) cout << '\n';
}

// Classe Data

// Diferenciadores de Construção
struct DiaTag {};
struct MesTag {};
struct AnoTag {};

class Data {
private:
    int dia, mes, ano;

public:
    Data() : dia(1), mes(1), ano(1) {}
    Data(int d, int m, int a) : dia(d), mes(m), ano(a) {}

    Data(int d, DiaTag) : dia(d), mes(1), ano(1) {}
    Data(int m, MesTag) : dia(1), mes(m), ano(1) {}
    Data(int a, AnoTag) : dia(1), mes(1), ano(a) {}

    int getDia() const {return dia;}
    int getMes() const {return mes;}
    int getAno() const {return ano;}

    void setDia(int d);
    void setMes(int m);
    void setAno(int a);

    void setData(int d, int m, int a) {
        dia = d;
        mes = m;
        ano = a;
    }

    void leiaData();
    string escrevaData() const;

    bool ehBissexto() const;
    bool dataValida() const;
    bool mesIgual(int mesParametrizado) const;

    string mesExtenso() const;
    int diasMes() const;
};

class DataHoje : public Data {
public:
    void leiaHoje() {
        time_t agora = time(nullptr);
        tm* tempoLocal = localtime(&agora);

        setDia(tempoLocal->tm_mday);
        setMes(tempoLocal->tm_mon + 1);
        setAno(tempoLocal->tm_year + 1900);
    }
};

void Data::setDia(int d) {
    if (d >= 1 && d <= 31) {
        dia = d;
    } else {
        cout << "Dia Inválido!" << endl;
    }
}

void Data::setMes(int m) {
    if (m >= 1 && m <= 12) {
        mes = m;
    } else {
        cout << "Mês Inválido!" << endl;
    }
}

void Data::setAno(int a) {
    if (a >= 1900 && a <= 2100) {
        ano = a;
    } else {
        cout << "Ano Inválido!" << endl;
    }
}

void Data::leiaData() {
    char separador;
    bool dataValida = false;

    while (!dataValida) {
        cout << "Data [dd/mm/aaaa (Sem '0' na Frente do Mês ou Dia)]: ";

        if (cin >> dia >> separador >> mes >> separador >> ano) {
            Utils::limparBuffer();

            if (this->dataValida()) {
                dataValida = true;
            } else {
                cout << "Data Inválida! Tente Novamente." << endl;
            }
        } else {
            cout << "Formato Inválido! Use dd/mm/aaaa" << endl;

            cin.clear();

            Utils::limparBuffer();
        }
    }
}

string Data::escrevaData() const {
    return to_string(dia) + "/" + to_string(mes) + "/" + to_string(ano);
}

bool Data::ehBissexto() const {
    return ((ano % 4 == 0 && ano % 100 != 0) || ano % 400 == 0);
}

bool Data::dataValida() const {
    bool valido = false;

    if (ano >= 1900 && ano <= 2100) {
        if (mes >= 1 && mes <= 12) {
            if (dia >= 1 && dia <= diasMes()) {
                valido = true;
            }
        }
    }

    return valido;
}

bool Data::mesIgual(int mesParametrizado) const {
    return (mes == mesParametrizado);
}

string Data::mesExtenso() const {
    string nomes[] = {
        "janeiro", "fevereiro", "março", "abril", "maio", "junho",
        "julho", "agosto", "setembro", "outubro", "novembro", "dezembro"
    };

    if (mes < 1 || mes > 12) return "Mês Inválido";
    return nomes[mes - 1];
}

int Data::diasMes() const {
    int dias_por_mes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (mes == 2 && ehBissexto()) return 29;
    if (mes < 1 || mes > 12) return 0;

    return dias_por_mes[mes - 1];
}

// Classe Pessoa
class Pessoa {
private:
    string nome;
    string cpf;
    Data nascimento;

public:
    Pessoa() : nome(""), cpf(""), nascimento() {}
    Pessoa(const string& n, const string& c, int d, int m, int a)
            : nome(n), cpf(c), nascimento(d, m, a) {}
    virtual ~Pessoa() {}

    string getNome() const {return nome;}
    string getCpf() const {return cpf;}
    Data getNascimento() const {return nascimento;}

    void setNome(const string& n) {nome = n;}
    void setCpf (const string& c) {cpf = c;}
    void setNascimento (Data d) {nascimento = d;}

    virtual void leiaDados();
    virtual void escrevaDados();
    virtual string getTipo() const = 0;
};

void Pessoa::leiaDados() {
    cout << "LEITURA DE DADOS:" << endl;

    nome = Utils::lerString("Nome: ");
    cpf = Utils::lerString("CPF [xxx.xxx.xxx-xx]: ");

    cout << "Nascimento - ";
    nascimento.leiaData();
}

void Pessoa::escrevaDados() {
    cout << "DADOS DA PESSOA:" << endl;
    cout << "Nome: " << getNome() << endl;
    cout << "CPF: " << getCpf() << endl;
    cout << "Data de Nascimento: " << getNascimento().escrevaData() << endl;
}

// Classe Aluno
class Aluno : public Pessoa {
private:
    string matricula;

public:
    static int quantidade;

    Aluno() {}
    Aluno(const string& n, const string& c, int d, int m, int a, const string& mat)
        : Pessoa(n, c, d, m, a), matricula(mat) {quantidade++;}
    virtual ~Aluno() {quantidade--;}

    string getMatricula() const {return matricula;}
    void setMatricula(const string& mat) {matricula = mat;}

    void leiaDados() override;
    void escrevaDados() override;
    string getTipo() const override {return "Aluno";}
};

int Aluno::quantidade = 0;

void Aluno::leiaDados() {
    Pessoa::leiaDados();
    matricula = Utils::lerString("Número de Matricula: ");
}

void Aluno::escrevaDados() {
    Pessoa::escrevaDados();
    cout << "Matricula: " << getMatricula() << endl;
}

// Classe Professor
class Professor : public Pessoa {
private:
    string titulo;

public:
    static int quantidade;
    static string titulosValidos[];

    Professor() {quantidade++;}
    Professor(const string& n, const string& c, int d, int m, int a, const string& tit)
        : Pessoa(n, c, d, m, a), titulo(tit) {quantidade++;}
    virtual ~Professor() {quantidade--;}

    string getTitulo() const {return titulo;}
    void setTitulo(const string& tit) {titulo = tit;}

    void leiaDados() override;
    void escrevaDados() override;
    string getTipo() const override {return "Professor";}
};

int Professor::quantidade = 0;
string Professor::titulosValidos[] = { "Especialista", "Mestre", "Doutor" };

void Professor::leiaDados() {
    Pessoa::leiaDados();

    int opc;
    bool valido = false;

    do {
        cout << "TITULAÇÃO: " << endl;
        cout << "1 - Especialista" << endl;
        cout << "2 - Mestre" << endl;
        cout << "3 - Doutor" << endl;
        cout << "Escolha: ";
        opc = Utils::lerOpc();

        if (opc >= 1 && opc <= 3) {
            titulo = titulosValidos[opc - 1];
            valido = true;
        } else {
            cout << "Opção Inválida!" << endl;
        }

    } while (!valido);
}

void Professor::escrevaDados() {
    Pessoa::escrevaDados();
    cout << "Titulação: " << getTitulo() << endl;
}

// Arrays Globais de Alunos e Professores
Aluno* alunos[MAX] = {nullptr};
Professor* professores[MAX] = {nullptr};

// Classe de Gerenciamento de Dados
class GerenciamentoDados {
public:
    static void carregarAlunos();
    static void carregarProfessores();

    static void salvarAlunos();
    static void salvarProfessores();
};

// Persistência dos Dados dos Alunos - Carregamento (Gerenciamento de Dados)
void GerenciamentoDados::carregarAlunos() {
    ifstream arquivo("alunos.txt");

    if (!arquivo.is_open()) {
        cout << "[INFO] Arquivo de Alunos não Encontrado. Iniciando com Lista Vazia." << endl;
        return;
    }

    int quantidadeArquivo;
    if (!(arquivo >> quantidadeArquivo)) {
        cout << "[ERRO] Erro ao Ler Quantidade do Arquivo de Alunos." << endl;
        arquivo.close();
        return;
    }

    arquivo.ignore();

    int salvos = 0;

    for (int i = 0; i < quantidadeArquivo && i < MAX; i++) {
        string nome, cpf, matricula;
        int dia, mes, ano;

        if (getline(arquivo, nome) && !nome.empty() &&
            getline(arquivo, cpf) && !cpf.empty() &&
            (arquivo >> dia >> mes >> ano) &&
            arquivo.ignore() &&
            getline(arquivo, matricula) && !matricula.empty()) {

            try {
                alunos[salvos++] = new Aluno(nome, cpf, dia, mes, ano, matricula);
            } catch (const bad_alloc& e) {
                cout << "[ERRO] Erro de Memória ao Carregar Aluno: " << cpf << endl;
                break;
            }

            } else {
                cout << "[AVISO] Dados Incompletos no Arquivo para Aluno " << i + 1 << endl;
            }
    }

    Aluno::quantidade = salvos;

    arquivo.close();
    cout << "[INFO] Carregados " << Aluno::quantidade << " Aluno(s) do Arquivo." << endl;
}

// Persistência dos Dados dos Professor - Carregamento (Gerenciamento de Dados)
void GerenciamentoDados::carregarProfessores() {
    ifstream arquivo("professores.txt");

    if (!arquivo.is_open()) {
        cout << "[INFO] Arquivo de Professores não Encontrado. Iniciando com Lista Vazia." << endl;
        return;
    }

    int quantidadeArquivo;
    if (!(arquivo >> quantidadeArquivo)) {
        cout << "[ERRO] Erro ao Ler Quantidade do Arquivo de Professores." << endl;
        arquivo.close();
        return;
    }

    arquivo.ignore();

    int salvos = 0;

    for (int i = 0; i < quantidadeArquivo && i < MAX; i++) {
        string nome, cpf, titulo;
        int dia, mes, ano;

        if (getline(arquivo, nome) && !nome.empty() &&
            getline(arquivo, cpf) && !cpf.empty() &&
            (arquivo >> dia >> mes >> ano) &&
            arquivo.ignore() &&
            getline(arquivo, titulo) && !titulo.empty()) {

            try {
                professores[salvos++] = new Professor(nome, cpf, dia, mes, ano, titulo);
            } catch (const bad_alloc& e) {
                cout << "[ERRO] Erro de Memória ao Carregar Professor: " << cpf << endl;
                break;
            }

            } else {
                cout << "[AVISO] Dados Incompletos no Arquivo para Professor " << i + 1 << endl;
            }
    }

    Professor::quantidade = salvos;

    arquivo.close();
    cout << "[INFO] Carregados " << Professor::quantidade << " Professor(es) do Arquivo." << endl;
}

// Persistência dos Dados dos Alunos - Salvamento (Gerenciamento de Dados)
void GerenciamentoDados::salvarAlunos() {
    ofstream arquivo("alunos.txt");

    if (!arquivo.is_open()) {
        cout << "[ERRO] Erro ao Abrir Arquivo para Salvar Alunos!" << endl;
        return;
    }

    int salvos = 0;

    for (int i = 0; i < MAX; i++) {
        if (alunos[i] != nullptr) salvos++;
    }

    arquivo << salvos << endl;

    for (int i = 0; i < MAX; i++) {
        if (alunos[i] != nullptr) {
            arquivo << alunos[i]->getNome() << endl
                    << alunos[i]->getCpf() << endl
                    << alunos[i]->getNascimento().getDia() << " "
                    << alunos[i]->getNascimento().getMes() << " "
                    << alunos[i]->getNascimento().getAno() << endl
                    << alunos[i]->getMatricula() << endl;
        }
    }

    Aluno::quantidade = salvos;

    cout << "Salvando em: " << filesystem::current_path() << endl;
    arquivo.close();
    cout << "[INFO] " << salvos << " Alunos Salvos no Arquivo." << endl;

    if (salvos != Aluno::quantidade) {
        cout << "[AVISO] Inconsistência Detectada! Esperado: " << Aluno::quantidade << ", Salvos: " << salvos << endl;
    }
}

// Persistência dos Dados dos Professores - Salvamento (Gerenciamento de Dados)
void GerenciamentoDados::salvarProfessores() {
    ofstream arquivo("professores.txt");

    if (!arquivo.is_open()) {
        cout << "[ERRO] Erro ao Abrir Arquivo para Salvar Professores!" << endl;
        return;
    }

    int salvos = 0;
    for (int i = 0; i < MAX; i++) {
        if (professores[i] != nullptr) salvos++;
    }

    arquivo << salvos << endl;

    for (int i = 0; i < MAX; i++) {
        if (professores[i] != nullptr) {
            arquivo << professores[i]->getNome() << endl
                    << professores[i]->getCpf() << endl
                    << professores[i]->getNascimento().getDia() << " "
                    << professores[i]->getNascimento().getMes() << " "
                    << professores[i]->getNascimento().getAno() << endl
                    << professores[i]->getTitulo() << endl;
        }
    }

    Professor::quantidade = salvos;

    cout << "Salvando em: " << filesystem::current_path() << endl;
    arquivo.close();
    cout << "[INFO] " << salvos << " Professores Salvos no Arquivo." << endl;

    if (salvos != Professor::quantidade) {
        cout << "[AVISO] Inconsistência detectada! Esperado: " << Professor::quantidade << ", Salvos: " << salvos << endl;
    }
}

// Classe de Operações do Sistema
class Sistema {
public:
    // Cadastro
    void cadastrarProfessor();
    void cadastrarAluno();

    // Pesquisa por Nome
    void pesquisarPorNomeProfessor();
    void pesquisarPorNomeAluno();

    // Pesquisa por CPF
    void pesquisarPorCpfProfessor();
    void pesquisarPorCpfAluno();

    // Deletar por CPF
    void excluirPorCpfProfessor();
    void excluirPorCpfAluno();

    // Apaga Todos os Registros
    void excluirTodosProfessores();
    void excluirTodosAlunos();
    void excluirTodos();

    // Listar Pessoas
    void listarTodos();

    void listarTodosAlunos();
    bool listarTodosAlunosRec(int i);

    void listarTodosProfessores();
    bool listarTodosProfessoresRec(int i);

    // Listar Aniversariantes do Mês
    void listarTodosAniversariantes(int mesInformado);
    void listarAniversariantesAlunos(int mesInformado);
    void listarAniversariantesProfessores(int mesInformado);
};

// Cadastro de Aluno (Sistema)
void Sistema::cadastrarAluno() {
    try {
        if (Aluno::quantidade >= MAX) {
            throw runtime_error("[AVISO] Limite de Alunos Atingido! Retornando ao Menu Principal.");
        }

        cout << "CADASTRO DE ALUNO:" << endl;

        Aluno temp;
        temp.leiaDados();

        Aluno* novoAluno = new Aluno(temp.getNome(), temp.getCpf(),
                                     temp.getNascimento().getDia(),
                                     temp.getNascimento().getMes(),
                                     temp.getNascimento().getAno(),
                                     temp.getMatricula());

        for (int i = 0; i < MAX; i++) {
            if (alunos[i] == nullptr) {
                alunos[i] = novoAluno;
                break;
            }
        }

        GerenciamentoDados::salvarAlunos();

        cout << "Novo Aluno Cadastrado com Sucesso!" << endl;
    } catch (const bad_alloc& e) {
        cout << "[ERRO] Memória Insuficiente para Criar Novo Aluno. Retornando ao Menu Principal." << endl;
        return;
    } catch (const runtime_error& e) {
        cout << e.what() << endl;
        return;
    } catch (...) {
        cout << "[AVISO] Erro Desconhecido ao Cadastrar Aluno. Retornando ao Menu Principal." << endl;
        return;
    }

    Utils::imprimirLinha();
}

// Cadastro de Professor (Sistema)
void Sistema::cadastrarProfessor() {
    try {
        if (Professor::quantidade >= MAX) {
            throw runtime_error("[AVISO] Limite de Professores Atingido! Retornando ao Menu Principal.");
        }

        cout << "CADASTRO DE PROFESSOR:" << endl;

        Professor temp;
        temp.leiaDados();

        Professor* novoProfessor = new Professor(temp.getNome(), temp.getCpf(),
                                                 temp.getNascimento().getDia(),
                                                 temp.getNascimento().getMes(),
                                                 temp.getNascimento().getAno(),
                                                 temp.getTitulo());

        for (int i = 0; i < MAX; i++) {
            if (professores[i] == nullptr) {
                professores[i] = novoProfessor;
                break;
            }
        }

        GerenciamentoDados::salvarProfessores();

        cout << "Novo Professor Cadastrado com Sucesso!" << endl;
    } catch (const bad_alloc& e) {
        cout << "[ERRO] Memória Insuficiente para Criar Novo Professor. Retornando ao Menu Principal." << endl;
        return;
    } catch (const runtime_error& e) {
        cout << e.what() << endl;
        return;
    } catch (...) {
        cout << "[AVISO] Erro Desconhecido ao Cadastrar Professor. Retornando ao Menu Principal." << endl;
        return;
    }

    Utils::imprimirLinha();
}

// Pesquisa por Nome de Aluno (Sistema)
void Sistema::pesquisarPorNomeAluno() {
    cout << "PESQUISA DE ALUNO POR NOME:" << endl;

    if (Aluno::quantidade == 0) {
        cout << "Nenhum Aluno Cadastrado!" << endl;
        return;
    }

    string nomeBusca;
    cout << "Nome para Pesquisa: ";
    getline(cin, nomeBusca);

    bool encontrou = false;
    for (int i = 0; i < MAX; i++) {
        if (alunos[i] != nullptr) {
            string nomeAluno = alunos[i]->getNome();

            if (nomeAluno.find(nomeBusca) != string::npos) {
                cout << "[" << i + 1 << "]" << endl;
                alunos[i]->escrevaDados();
                encontrou = true;
            }
        }
    }

    if (!encontrou) {
        cout << "Nenhum Aluno Encontrado com esse Nome." << endl;
    }

    Utils::imprimirLinha();
}

// Pesquisa por Nome de Professor (Sistema)
void Sistema::pesquisarPorNomeProfessor() {
    cout << "PESQUISA DE PROFESSOR POR NOME:" << endl;

    if (Professor::quantidade == 0) {
        cout << "Nenhum Professor Cadastrado!" << endl;
        return;
    }

    string nomeBusca;
    cout << "Nome para Pesquisa: ";
    getline(cin, nomeBusca);

    bool encontrou = false;
    for (int i = 0; i < MAX; i++) {
        if (professores[i] != nullptr) {
            string nomeProf = professores[i]->getNome();

            if (nomeProf.find(nomeBusca) != string::npos) {
                cout << "[" << i + 1 << "]" << endl;
                professores[i]->escrevaDados();
                encontrou = true;
            }
        }
    }

    if (!encontrou) {
        cout << "Nenhum Professor Encontrado com esse Nome." << endl;
    }

    Utils::imprimirLinha();
}

// Pesquisa por CPF de Aluno (Sistema)
void Sistema::pesquisarPorCpfAluno() {
    cout << "PESQUISA DE ALUNO POR CPF:" << endl;

    if (Aluno::quantidade == 0) {
        cout << "Nenhum Aluno Cadastrado!" << endl;
        return;
    }

    string cpfBusca;
    cout << "CPF para Pesquisa [xxx.xxx.xxx-xx]: ";
    getline(cin, cpfBusca);

    string cpfLimpoBusca = Utils::limparCPF(cpfBusca);

    bool encontrou = false;
    for (int i = 0; i < MAX; i++) {
        if (alunos[i] != nullptr) {
            string cpfAluno = alunos[i]->getCpf();
            string cpfAlunoLimpo = Utils::limparCPF(cpfAluno);

            if (cpfAlunoLimpo == cpfLimpoBusca) {
                cout << "[" << i + 1 << "]" << endl;
                alunos[i]->escrevaDados();
                encontrou = true;
            }
        }
    }

    if (!encontrou) {
        cout << "Nenhum Aluno Encontrado com esse CPF." << endl;
    }

    Utils::imprimirLinha();
}

// Pesquisa por CPF de Professor (Sistema)
void Sistema::pesquisarPorCpfProfessor() {
    cout << "PESQUISA DE PROFESSOR POR CPF:" << endl;

    if (Professor::quantidade == 0) {
        cout << "Nenhum Professor Cadastrado!" << endl;
        return;
    }

    string cpfBusca;
    cout << "CPF para Pesquisa [xxx.xxx.xxx-xx]: ";
    getline(cin, cpfBusca);

    string cpfLimpoBusca = Utils::limparCPF(cpfBusca);

    bool encontrou = false;
    for (int i = 0; i < MAX; i++) {
        if (professores[i] != nullptr) {
            string cpfProf = professores[i]->getCpf();
            string cpfProfLimpo = Utils::limparCPF(cpfProf);

            if (cpfProfLimpo == cpfLimpoBusca) {
                cout << "[" << i + 1 << "]" << endl;
                professores[i]->escrevaDados();
                encontrou = true;
            }
        }
    }

    if (!encontrou) {
        cout << "Nenhum Professor Encontrado com esse CPF." << endl;
    }

    Utils::imprimirLinha();
}

// Excluir Aluno por CPF (Sistema)
void Sistema::excluirPorCpfAluno() {
    cout << "EXCLUIR ALUNO POR CPF:" << endl;

    if (Aluno::quantidade == 0) {
        cout << "Nenhum Aluno Cadastrado!" << endl;
        return;
    }

    string cpfBusca = Utils::lerString("CPF do Aluno a ser Excluído [xxx.xxx.xxx-xx]: ");
    string cpfLimpoBusca = Utils::limparCPF(cpfBusca);

    for (int i = 0; i < MAX; i++) {
        if (alunos[i] != nullptr && Utils::limparCPF(alunos[i]->getCpf()) == cpfLimpoBusca) {
            cout << "Aluno Encontrado: " << alunos[i]->getNome() << endl;

            delete alunos[i];
            alunos[i] = nullptr;
            Aluno::quantidade--;

            for (int j = i; j < MAX - 1; j++) {
                alunos[j] = alunos[j + 1];
                if (alunos[j] == nullptr) break;
            }

            alunos[MAX - 1] = nullptr;

            GerenciamentoDados::salvarAlunos();

            cout << "Aluno Excluído com Sucesso!" << endl;
            return;
        }
    }

    cout << "Aluno com esse CPF não Encontrado." << endl;
    Utils::imprimirLinha();
}

// Excluir Professor por CPF (Sistema)
void Sistema::excluirPorCpfProfessor() {
    cout << "EXCLUIR PROFESSOR POR CPF:" << endl;

    if (Professor::quantidade == 0) {
        cout << "Nenhum Aluno Cadastrado!" << endl;
        return;
    }

    string cpfBusca = Utils::lerString("CPF do Professor a ser Excluído [xxx.xxx.xxx-xx]: ");
    string cpfLimpoBusca = Utils::limparCPF(cpfBusca);

    for (int i = 0; i < MAX; i++) {
        if (professores[i] != nullptr && Utils::limparCPF(professores[i]->getCpf()) == cpfLimpoBusca) {
            cout << "Aluno Encontrado: " << professores[i]->getNome() << endl;

            delete professores[i];
            professores[i] = nullptr;
            Professor::quantidade--;

            for (int j = i; j < MAX - 1; j++) {
                professores[j] = professores[j + 1];
                if (professores[j] == nullptr) break;
            }

            professores[MAX - 1] = nullptr;

            GerenciamentoDados::salvarProfessores();

            cout << "Professor Excluído com Sucesso!" << endl;
            return;
        }
    }

    cout << "Professor com esse CPF não Encontrado." << endl;
    Utils::imprimirLinha();
}

// Listar Todas as Pessoas (Sistema)
void Sistema::listarTodos() {
    cout << "LISTA COMPLETA DE TODAS AS PESSOAS CADASTRADAS:" << endl;

    listarTodosProfessores();
    listarTodosAlunos();
}

// Listar Todos os Alunos (Sistema)
void Sistema::listarTodosAlunos() {
    cout << "LISTA DE TODOS OS ALUNOS CADASTRADOS:" << endl;
    bool encontrou = listarTodosAlunosRec(0);

    if (!encontrou) {
        cout << "Nenhum Aluno Cadastrado." << endl;
    }

    Utils::imprimirLinha();
}

bool Sistema::listarTodosAlunosRec(int i) {
    if (i >= MAX) return false;

    bool encontrouNoAtual = false;
    if (alunos[i] != nullptr) {
        cout << "[" << i + 1 << "]" << endl;
        alunos[i]->escrevaDados();
        encontrouNoAtual = true;
    }

    bool encontrouNoResto = listarTodosAlunosRec(i + 1);

    if (encontrouNoAtual) {
        return true;
    } else {
        return encontrouNoResto;
    }
}

// Listar Todos os Professores (Sistema)
void Sistema::listarTodosProfessores() {
    cout << "LISTA DE TODOS OS PROFESSORES CADASTRADOS:" << endl;
    bool encontrou = listarTodosProfessoresRec(0);

    if (!encontrou) {
        cout << "Nenhum Professor Cadastrado." << endl;
    }

    Utils::imprimirLinha();
}

bool Sistema::listarTodosProfessoresRec(int i) {
    if (i >= MAX) return false;

    bool encontrouNoAtual = false;
    if (professores[i] != nullptr) {
        cout << "[" << i + 1 << "]" << endl;
        professores[i]->escrevaDados();
        encontrouNoAtual = true;
    }

    bool encontrouNoResto = listarTodosProfessoresRec(i + 1);

    if (encontrouNoAtual) {
        return true;
    } else {
        return encontrouNoResto;
    }
}

// Lista Todos os Aniversariantes do Mês (Sistema)
void Sistema::listarTodosAniversariantes(int mesParametrizado) {
    vector<Pessoa*> aniversariantes;

    for (int i = 0; i < Aluno::quantidade; i++) {
        if (alunos[i] != nullptr && alunos[i]->getNascimento().getMes() == mesParametrizado) {
            aniversariantes.push_back(alunos[i]);
        }
    }

    for (int i = 0; i < Professor::quantidade; i++) {
        if (professores[i] != nullptr && professores[i]->getNascimento().getMes() == mesParametrizado) {
            aniversariantes.push_back(professores[i]);
        }
    }

    cout << "TODOS OS ANIVERSARIANTES DO MÊS: " << mesParametrizado << ":\n";

    if (aniversariantes.empty()) {
        cout << "Nenhum Aniversariante Encontrado." << endl;
    } else {
        for (Pessoa* p : aniversariantes) {
            cout << "[" << p->getTipo() << "]" << endl;
            p->escrevaDados();
        }
    }

    Utils::imprimirLinha();
}

// Lista Todos os Alunos Aniversariantes do Mês (Sistema)
void Sistema::listarAniversariantesAlunos(int mesParametrizado) {
    bool encontrou = false;

    cout << "ALUNOS ANIVERSARIANTES DO MÊS" << mesParametrizado << ":" << endl;

    for (int i = 0; i < MAX; i++) {
        if (alunos[i] != nullptr) {
            if (alunos[i]->getNascimento().mesIgual(mesParametrizado)) {
                cout << "[" << i + 1 << "]" << endl;
                alunos[i]->escrevaDados();
                encontrou = true;
            }
        }
    }

    if (!encontrou) {
        cout << "Nenhum Aluno Aniversariante Encontrado Neste Mês." << endl;
    }

    Utils::imprimirLinha();
}

// Lista Todos os Professores Aniversariantes do Mês (Sistema)
void Sistema::listarAniversariantesProfessores(int mesParametrizado) {
    bool encontrou = false;

    cout << "PROFESSORES ANIVERSARIANTES DO MÊS" << mesParametrizado << ":" << endl;

    for (int i = 0; i < MAX; i++) {
        if (professores[i] != nullptr) {
            if (professores[i]->getNascimento().mesIgual(mesParametrizado)) {
                cout << "[" << i + 1 << "]" << endl;
                professores[i]->escrevaDados();
                encontrou = true;
            }
        }
    }

    if (!encontrou) {
        cout << "Nenhum Professor Aniversariante Encontrado Neste Mês." << endl;
    }

    Utils::imprimirLinha();
}

// Apaga Todos os Registros de Professores do Sistema (Sistema)
void Sistema::excluirTodosProfessores() {
    cout << "APAGAR TODOS OS DADOS DE PROFESSORES:" << endl;

    if (Professor::quantidade == 0) {
        cout << "Nenhum Professor Cadastrado!" << endl;
        return;
    } else {
        for (int i = 0; i < MAX; i++) {
            delete professores[i];
            professores[i] = nullptr;
        }

        Professor::quantidade = 0;
    }

    GerenciamentoDados::salvarProfessores();

    cout << "Todos os Professores Foram Excluídos com Sucesso." << endl;
    Utils::imprimirLinha();
}

// Apaga Todos os Registros de Alunos do Sistema (Sistema)
void Sistema::excluirTodosAlunos() {
    cout << "APAGAR TODOS OS DADOS DE ALUNOS:" << endl;

    if (Aluno::quantidade == 0) {
        cout << "Nenhum Aluno Cadastrado!" << endl;
        return;
    } else {
        for (int i = 0; i < MAX; i++) {
            delete alunos[i];
            alunos[i] = nullptr;
        }

        Aluno::quantidade = 0;
    }

    GerenciamentoDados::salvarAlunos();

    cout << "Todos os Alunos Foram Excluídos com Sucesso." << endl;
    Utils::imprimirLinha();
}

// Apaga Todos os Registros de Pessoas do Sistema (Sistema)
void Sistema::excluirTodos() {
    cout << "APAGAR TODOS OS DADOS:" << endl;

    excluirTodosProfessores();
    excluirTodosAlunos();

    cout << "Todos os Dados (Professores e Alunos) Foram Excluídos." << endl;
    Utils::imprimirLinha();
}

// Gerenciamento de Opções - Menus e Submenus
class Menus {
public:
    // Status
    void status();

    // Menu
    void menuPrincipal();

    // Submenus de Cadastro e Listagem
    void submenuCadastro();
    void submenuListar();

    // Submenus de Pesquisa
    void submenuPesquisarNome();
    void submenuPesquisarCpf();

    // Submenus de Deletar
    void submenuExcluir();
    void submenuApagarTodos();

    // Submenus de Aniversariantes
    void submenuAniversariantes();
};

// Exibe Dados sobre a Quantidade de Pessoas Registradas no Sistema (Menu)
void Menus::status() {
    int total = Aluno::quantidade + Professor::quantidade;

    cout << "STATUS DO PROGRAMA" << endl;
    cout << "Quantidade Total de Pessoas Cadastradas: " << total << endl;

    cout << "Professores: " << Professor::quantidade << endl;
    cout << "Alunos: " << Aluno::quantidade << endl;

    Utils::imprimirLinha();
}

// Menus Principal (Menu)
void Menus::menuPrincipal() {
    int escolha;

    cout << "MENU PRINCIPAL" << endl;
    cout << "0 - Salvar Dados" << endl;
    cout << "1 - Cadastrar uma Pessoa" << endl;
    cout << "2 - Listar Todas as Pessoas" << endl;
    cout << "3 - Pesquisar por Nome" << endl;
    cout << "4 - Pesquisar por CPF" << endl;
    cout << "5 - Excluir Pessoa" << endl;
    cout << "6 - Apagar Todas as Pessoas Cadastradas" << endl;
    cout << "7 - Aniversariantes do Mês" << endl;
    cout << "8 - Mostrar Status do Programa" << endl;
    cout << "9 - Sair do Programa" << endl;

    cout << "Escolha: ";
    escolha = Utils::lerOpc();

    Utils::imprimirLinha();

    switch (escolha) {
        case 0:
            GerenciamentoDados::salvarProfessores();
            GerenciamentoDados::salvarAlunos();

            cout << "Dados Salvos!" << endl;
            break;
        case 1:
            // Submenu de Cadastro de Alunos e Professores
            submenuCadastro();
            break;
        case 2:
            // Submenu de Listar Alunos e Professores
            submenuListar();
            break;
        case 3:
            // Submenu de Pesquisar Aluno ou Professor por Nome
            submenuPesquisarNome();
            break;
        case 4:
            // Submenu de Pesquisar Aluno ou Professor por CPF
            submenuPesquisarCpf();
            break;
        case 5:
            // Submenu de Excluir Aluno ou Professor por CPF
            submenuExcluir();
            break;
        case 6:
            // Submenu de Apagar Todos os Dados (Geral ou Especifico)
            submenuApagarTodos();
            break;
        case 7:
            // Submenu de Listar Aniversariantes de Determinado Mês
            submenuAniversariantes();
            break;
        case 8:
            // Mostrar Status do Programa
            status();
            break;
        case 9:
            // Sair do Programa
            break;
        default:
            cout << "Opção Inválida!" << endl;
            Utils::pausar();
            break;
    }
}

// Submenu de Cadastro de Aluno e Professor (Menu)
void Menus::submenuCadastro() {
    Sistema sistema;
    int escolha;

    do {
        cout << "SUBMENU - Cadastrar Pessoa" << endl;

        cout << "10 - Voltar ao Menu Anterior" << endl;
        cout << "11 - Cadastrar Professor" << endl;
        cout << "12 - Cadastrar Aluno" << endl;
        cout << "Escolha: ";
        escolha = Utils::lerOpc();

        Utils::imprimirLinha();

        switch (escolha) {
            case 10: break;
            case 11:
                sistema.cadastrarProfessor();
                break;
            case 12:
                sistema.cadastrarAluno();
                break;
            default:
                cout << "Opção Inválida!" << endl;
                Utils::pausar();
                break;
        }

    } while (escolha != 10);
}

// Submenu para Listar Pessoas (Menu)
void Menus::submenuListar() {
    Sistema sistema;
    int escolha;

    do {
        cout << "SUBMENU - Listar Pessoas" << endl;

        cout << "20 - Voltar ao Menu Anterior" << endl;
        cout << "21 - Listar Professores" << endl;
        cout << "22 - Listar Alunos" << endl;
        cout << "23 - Listar Todos" << endl;
        cout << "Escolha: ";
        escolha = Utils::lerOpc();

        Utils::imprimirLinha();

        switch (escolha) {
            case 20: break;
            case 21:
                sistema.listarTodosProfessores();
                break;
            case 22:
                sistema.listarTodosAlunos();
                break;
            case 23:
                sistema.listarTodos();
                break;
            default:
                cout << "Opção Inválida!" << endl;
                Utils::pausar();
                break;
        }

    } while (escolha != 20);
}

// Submenu para Pesquisar por Nome (Menu)
void Menus::submenuPesquisarNome() {
    Sistema sistema;
    int escolha;

    do {
        cout << "SUBMENU - Pesquisar Pessoa por Nome" << endl;

        cout << "30 - Voltar ao Menu Anterior" << endl;
        cout << "31 - Pesquisar Professor por Nome" << endl;
        cout << "32 - Pesquisar Aluno por Nome" << endl;
        cout << "Escolha: ";
        escolha = Utils::lerOpc();

        Utils::imprimirLinha();

        switch (escolha) {
            case 30: break;
            case 31:
                sistema.pesquisarPorNomeProfessor();
                break;
            case 32:
                sistema.pesquisarPorNomeAluno();
                break;
            default:
                cout << "Opção Inválida!" << endl;
                Utils::pausar();
                break;
        }

    } while (escolha != 30);
}

// Submenu para Pesquisar por CPF (Menu)
void Menus::submenuPesquisarCpf() {
    Sistema sistema;
    int escolha;

    do {
        cout << "SUBMENU - Pesquisar Pessoa por CPF" << endl;

        cout << "40 - Voltar ao Menu Anterior" << endl;
        cout << "41 - Pesquisar Professor por CPF" << endl;
        cout << "42 - Pesquisar Aluno por CPF" << endl;
        cout << "Escolha: ";
        escolha = Utils::lerOpc();

        Utils::imprimirLinha();

        switch (escolha) {
            case 40: break;
            case 41:
                sistema.pesquisarPorCpfProfessor();
                break;
            case 42:
                sistema.pesquisarPorCpfAluno();
                break;
            default:
                cout << "Opção Inválida!" << endl;
                Utils::pausar();
                break;
        }

    } while (escolha != 40);
}

// Submenu para Excluir Pessoa (Menu)
void Menus::submenuExcluir() {
    Sistema sistema;
    int escolha;

    do {
        cout << "SUBMENU - Excluir Pessoa Especifica" << endl;

        cout << "50 - Voltar ao Menu Anterior" << endl;
        cout << "51 - Excluir Professor por CPF" << endl;
        cout << "52 - Excluir Aluno por CPF" << endl;
        cout << "Escolha: ";
        escolha = Utils::lerOpc();

        Utils::imprimirLinha();

        switch (escolha) {
            case 50: break;
            case 51:
                sistema.excluirPorCpfProfessor();
                break;
            case 52:
                sistema.excluirPorCpfAluno();
                break;
            default:
                cout << "Opção Inválida!" << endl;
                Utils::pausar();
                break;
        }

    } while (escolha != 50);
}

// Submenu para Deletar Pessoas (Menu)
void Menus::submenuApagarTodos() {
    Sistema sistema;
    int escolha;

    do {
        cout << "SUBMENU - Apagar Todos os Registros de Pessoas" << endl;

        cout << "60 - Voltar ao Menu Anterior" << endl;
        cout << "61 - Excluir Todos os Professores" << endl;
        cout << "62 - Excluir Todos os Alunos" << endl;
        cout << "63 - Excluir Todos (Geral)" << endl;
        cout << "Escolha: ";
        escolha = Utils::lerOpc();

        Utils::imprimirLinha();

        switch (escolha) {
            case 60: break;
            case 61:
                sistema.excluirTodosProfessores();
                break;
            case 62:
                sistema.excluirTodosAlunos();
                break;
            case 63:
                sistema.excluirTodos();
                break;
            default:
                cout << "Opção Inválida!" << endl;
                Utils::pausar();
                break;
        }

    } while (escolha != 60);
}

// Submenu para Exibir Aniversariantes (Menu)
void Menus::submenuAniversariantes() {
    Sistema sistema;
    int escolha;

    bool mesJaInformado = false;
    int mesInformado = 0;

    do {
        cout << "SUBMENU - Exibir Aniversariantes do Mês" << endl;

        cout << "70 - Voltar ao Menu Anterior" << endl;
        cout << "71 - Informar o Mês a ser Pesquisado" << endl;

        if (mesJaInformado) {
            cout << "72 - Listar os Professores Aniversariantes do Mês " << mesInformado << endl;
            cout << "73 - Listar os Alunos Aniversariantes do Mês " << mesInformado << endl;
            cout << "74 - Listar Todos os Aniversariantes do Mês " << mesInformado << endl;
        } else {
            cout << "72 - Listar os Professores Aniversariantes (Informe o Mês Primeiro)" << endl;
            cout << "73 - Listar os Alunos Aniversariantes (Informe o Mês Primeiro)" << endl;
        }

        cout << "Escolha: ";
        escolha = Utils::lerOpc();

        Utils::imprimirLinha();

        switch (escolha) {
            case 70: break;
            case 71: {
                cout << "Digite o Mês (1-12): ";
                int mes = Utils::lerOpc();

                if (mes >= 1 && mes <= 12) {
                    mesInformado = mes;
                    mesJaInformado = true;
                    cout << "Mês [" << mes << "] Selecionado com Sucesso!" << endl;
                } else {
                    cout << "Mês Inválido! Digite um Valor Entre 1 e 12." << endl;
                }

                Utils::pausar();
                break;
            }

            case 72:
                if (mesJaInformado) {
                    sistema.listarAniversariantesProfessores(mesInformado);
                    break;
                } else {
                    cout << "Por Favor, Informe o Mês Primeiro (opção 71)!" << endl;
                }

                Utils::pausar();
                break;
            case 73:
                if (mesJaInformado) {
                    sistema.listarAniversariantesAlunos(mesInformado);
                    break;
                } else {
                    cout << "Por Favor, Informe o Mês Primeiro (opção 71)!" << endl;
                }

                Utils::pausar();
                break;
            case 74:
                if (mesJaInformado) {
                    sistema.listarTodosAniversariantes(mesInformado);
                } else {
                    cout << "Por Favor, Informe o Mês Primeiro (opção 71)!" << endl;
                }

                Utils::pausar();
                break;
            default:
                cout << "Opção Inválida!" << endl;
                Utils::pausar();
                break;
        }

    } while (escolha != 70);
}

// Função Principal
int main() {
    Utils::imprimirLinha();

    // Leitura do Dia Atual
    DataHoje hoje;
    hoje.leiaHoje();

    Utils::imprimirLinha();

    // Exibição da Data do Dia
    cout << "Data de Hoje: " << hoje.escrevaData() << endl;

    Utils::imprimirLinha();

    // Inicialização do Carregamento de Dados de Alunos e Professores
    GerenciamentoDados::carregarAlunos();
    GerenciamentoDados::carregarProfessores();

    Utils::imprimirLinha();

    // Aviso para Melhor Funcionamento
    cout << "[IMPORTANTE] Para evitar bugs ou informações erradas exibidas nas \n"
            "funcionalidades, sempre salve os dados após cadastrar um aluno ou \n"
            "professor, bem como após de deleta-lo, para que o programa esteja "
            "\nsempre atualizado!" << endl;

    Utils::imprimirLinha();

    // Criação de Objeto da Classe Menu
    Menus menu;
    int escolha;

    // Loop de Escolha
    do {
        menu.menuPrincipal();

        bool entradaValida = false;

        do {
            cout << "Confirmação - Deseja Continuar?" << endl;
            cout << "0 - Não" << endl;
            cout << "1 - Sim" << endl;
            cout << "Escolha: ";
            escolha = Utils::lerOpc();

            Utils::imprimirLinha();

            if (escolha == 1) {
                Utils::limparTela();
                entradaValida = true;
            } else if (escolha == 0) {
                entradaValida = true;
            } else {
                cout << "Escolha Inválida! Tente Novamente." << endl;
            }
        } while (!entradaValida);

    } while (escolha != 0);

    // Salvamento ao Final da Execução
    GerenciamentoDados::salvarProfessores();
    GerenciamentoDados::salvarAlunos();

    // Limpando a Memoria
    for (int i = 0; i < Professor::quantidade; i++) {
        if (professores[i] != nullptr) {
            delete professores[i];
        }
    }

    for (int i = 0; i < Aluno::quantidade; i++) {
        if (alunos[i] != nullptr) {
            delete alunos[i];
        }
    }

    // Mensagem de Encerramento
    cout << "Programa Encerrado com Sucesso." << endl;

    Utils::imprimirLinha();

    return 0;
}