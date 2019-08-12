REM Software pw3270, desenvolvido com base nos códigos fontes do WC3270  e X3270
REM (Paul Mattes Paul.Mattes@usa.net), de emulação de terminal 3270 para acesso a
REM aplicativos mainframe. Registro no INPI sob o nome G3270.
REM 
REM Copyright (C) <2008> <Banco do Brasil S.A.>
REM 
REM Este programa é software livre. Você pode redistribuí-lo e/ou modificá-lo sob
REM os termos da GPL v.2 - Licença Pública Geral  GNU,  conforme  publicado  pela
REM Free Software Foundation.
REM 
REM Este programa é distribuído na expectativa de  ser  útil,  mas  SEM  QUALQUER
REM GARANTIA; sem mesmo a garantia implícita de COMERCIALIZAÇÃO ou  de  ADEQUAÇÃO
REM A QUALQUER PROPÓSITO EM PARTICULAR. Consulte a Licença Pública Geral GNU para
REM obter mais detalhes.
REM 
REM Você deve ter recebido uma cópia da Licença Pública Geral GNU junto com este
REM programa; se não, escreva para a Free Software Foundation, Inc., 51 Franklin
REM St, Fifth Floor, Boston, MA  02110-1301  USA
REM 
REM Contatos:
REM 
REM perry.werneck@gmail.com      (Alexandre Perry de Souza Werneck)
REM erico.mendonca@gmail.com     (Erico Mascarenhas Mendonça)
REM 

REM Inicializa a biblioteca 

rem hllapi_init() - Inicia uma sessão 3270
rem
rem hllapi_init pode receber como argumento o nome de uma janela já aberta,
rem por exemplo, "pw3270:a" para com que atue direto sobre ela, ou,
rem com uma string vazia ("") para tentar criar uma sessão sem janela.
rem
Declare Function hllapi_init Lib "libhllapi.dll" (ByVal tp as string) as Long 

rem hllapi_deinit() - Encerra uma sessão 3270
rem
rem Finaliza a sessão 3270 liberando os recursos usados.
rem
Declare Function hllapi_deinit Lib "libhllapi.dll" () as Long 

rem hllapi_get_revision - Obtém a revisão do pw3270 que está sendo usada.
rem
Declare Function hllapi_get_revision Lib "libhllapi.dll" () as Long 

rem hllapi_connect() - Conecta a um host 3270
rem
rem Efetua a conexão ao host 3270, recebe 2 parâmetros
rem
rem uri  - URI com o endereço de conexão ao host
rem wait - Quantos segundos a função deve esperar até o estabelecimento da sessão
rem
Declare Function hllapi_connect Lib "libhllapi.dll" (ByVal uri as string, ByVal wait as Integer) as Long 

rem hllapi_disconnect() - Desconecta do host 3270
rem
Declare Function hllapi_disconnect Lib "libhllapi.dll" () as Long 

rem hllapi_wait_for_ready() - Aguarda que o terminal fique disponível
rem
rem Suspende a atividade até que o terminal esteja pronto para receber comandos.
rem
rem Recebe apenas um valor numérico com o tempo em segundos a esperar; caso esse
rem tempo seja atingido sem o terminal ficar disponível a função retorna erro.
rem
Declare Function hllapi_wait_for_ready Lib "libhllapi.dll" (ByVal timeout as Integer) as Long

rem hllapi_wait() - Aguarda por "x" segundos
rem
rem Suspende a atividade do script mantendo a comunicação com o host ativa; pode ser usada
rem para parar por um tempo específico enquanto o host executa algum processo.
rem
rem Recebe apenas um valor numérico com o tempo em segundos a esperar.
rem
Declare Function hllapi_wait Lib "libhllapi.dll" (ByVal timeout as Integer) as Long 

rem hllapi_get_message_id() - Obtém o código da mensagem atual da OIA
rem
rem Obtém o código numérico correspondente à mensagem de status da OIA.
rem
Declare Function hllapi_get_message_id Lib "libhllapi.dll" () as Long 

rem hllapi_is_connected() - Verifica se existe uma conexão com o host
rem
Declare Function hllapi_is_connected Lib "libhllapi.dll" () as Long 

rem hllapi_get_screen() - Obtém conteúdo de uma parte do terminal pelo endereço
rem
rem Preenche a string com o conteúdo do termina na posição desejada
rem
rem Recebe 3 parâmetros:
rem
rem - Posição na tela
rem - String com espaço suficiente
rem - Tamanho desejado (se for -1 usa o tamanho da string)
rem
Declare Function hllapi_get_screen Lib "libhllapi.dll" (ByVal offset as Integer, ByVal text as String, ByVal length as Integer) as Long 

rem hllapi_get_screen_at() - Obtém o conteúdo de uma parte da tela do terminal.
rem
rem Preenche a string com o conteúdo do terminal a partir da linha e coluna
rem desejadas.
rem
rem Recebe 3 parâmetros:
rem
rem - Linha da posição inicial
rem - Coluna da posição inicial
rem - String já com o tamanho desejado
rem
Declare Function hllapi_get_screen_at Lib "libhllapi.dll" (ByVal row as Integer, ByVal col as Integer, ByVal text as String) as Long 

rem hllapi_set_text_at() - Aplica string na tela do terminal.
rem
rem Aplica a string na linha e coluna informadas; atentar para o fato de que
rem a posição deve estar disponível para escrita.
rem
rem Recebe 3 parâmetros:
rem
rem - Linha da posição inicial
rem - Coluna da posição inicial
rem - String que será aplicada
rem
Declare Function hllapi_set_text_at Lib "libhllapi.dll" (ByVal row as Integer, ByVal col as Integer, ByVal text as String) as Long 

rem hllapi_cmp_text_at() - Compara conteúdo da tela.
rem
rem Compara o conteúdo da tela na posição informada, recebe 3 parâmetros.
rem
rem - Linha da posição inicial
rem - Coluna da posição inicial
rem - String a comparar
rem
rem Retorna 0 se o valor contido na tela é igual ao informado, diferente de zero se não for.
rem
Declare Function hllapi_cmp_text_at Lib "libhllapi.dll" (ByVal row as Integer, ByVal col as Integer, ByVal text as String) as Long 

rem hllapi_find_text
rem
rem Obtém a posição da string na tela.
rem
rem - String a pesquisar
rem
rem 0 se a string não foi encontrada, a posição na tela se encontrar.
rem
Declare Function hllapi_find_text Lib "libhllapi.dll" (ByVal text as String) as Long 

rem hllapi_enter()
rem
rem Envia um [ENTER] para o host.
rem
Declare Function hllapi_enter Lib "libhllapi.dll" () as Long 

rem hllapi_pfkey()
rem
rem Envia uma [PF] para o host.
rem
rem Recebe como argumento o número da PF que será enviada.
rem
Declare Function hllapi_pfkey Lib "libhllapi.dll" (ByVal keycode as Integer) as Long

rem hllapi_pakey()
rem
rem Envia uma [PA] para o host.
rem
rem Recebe como argumento o número da PA que será enviada.
rem
Declare Function hllapi_pakey Lib "libhllapi.dll" (ByVal keycode as Integer) as Long

rem hllapi_set_session_parameter()
rem
rem Configura um parâmetro da sessão
rem 
rem Recebe como argumentos:
rem
rem   1. Nome do parâmetro a ser ajustado
rem   2. Tamanho do nome do parâmetro (ou 0)
rem   3. Valor a atribuir
rem
Declare Function hllapi_set_session_parameter Lib "libhllapi.dll" (ByVal text as String, ByVal len as Integer, ByVal value as Integer) as Long

rem hllapi_set_charset()
rem
rem Configura a página de código
rem 
rem Recebe como argumentos:
rem
rem   1. Nome da página de código desejada (UTF-8, CP1252, ISO-8859-1, etc).
rem
Declare Function hllapi_set_charset Lib "libhllapi.dll" (ByVal text as String, ByVal len as Integer, ByVal value as Integer) as Long

rem hllapi_set_unlock_delay()
rem
rem Define o tempo (em milisegundos) que o terminal
rem espera para desbloquear a sessão após um comando.
rem
rem Recebe como argumento o valor do delay em milisegundos.
rem
Declare Function hllapi_set_unlock_delay Lib "libhllapi.dll" (ByVal delay as Integer) as Long

rem hllapi_get_cursor_address() - Obtém a posição atual do cursor
rem
rem Obtém a posição atual do cursor dentro da tela do terminal
rem
Declare Function hllapi_get_cursor_address Lib "libhllapi.dll" () as Long

rem hllapi_set_cursor_address() 
rem 
rem Define a posição atual do cursor
rem
rem Recebe 1 argumento:
rem 
rem Novo endereço do cursor dentro da tela do terminal.
rem
Declare Function hllapi_set_cursor_address Lib "libhllapi.dll" (ByVal addr) as Long 

Sub Main 

    if hllapi_init("") <> 0 then 
        print "Erro ao inicializar a biblioteca" 
        Exit Sub 
    End If 
   	
    hllapi_connect("fandezhi.efglobe.com:23",0) 
  
    Do While hllapi_is_connected() = 0 
  
        hllapi_wait(1) 
  
    Loop
    
    if hllapi_wait_for_ready(60) <> 0 then 
        print "Conexão não se estabilizou no tempo esperado" 
        Exit Sub  
 	End If 
 	
    Dim text as string
    text = space(2000) 
 	
    if hllapi_get_screen_at(1,1,text) <> 0 then 
        print "Erro ao tentar obter conteudo do terminal" 
        Exit Sub 
    End If 
 	
    print text
    
    hllapi_disconnect() 
    hllapi_deinit()
 	
End Sub

