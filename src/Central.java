/**
 * @brief Este código implementa a central de comunicação da solução de estacionamento baseada em comunicação V2I.
 * Para mais detalhes acesse: https://github.com/minimiz3d/parking-solution 
 * @file Central.java
 * @author Árthur Tolfo Pinheiro
 * @date 2018-03-10
 */

import java.io.*;
import java.net.*;
import java.nio.CharBuffer;
import java.util.*;

/*	Compilar: 					javac Central.java 
	Rodar: 						java -cp utilities.jar;. Central
*/

/**
 * @brief Classe que implementa a comunicação com o veículo no estacionamento.
 */
public class Central {
	/**
	 * @brief Método principal que executa a comunicação num todo.
	 */
    public static void main(String argv[]) {
		String HOST 			= "192.168.0.101";
		int PORT 				= 8070;
		int timeout				= 20000;
		Socket socketClient 	= null;
		BufferedReader reader 	= null;
        DataOutputStream writer = null;
		String msg              = null;
		String msgToSend		= null;
		
		while (true) {
			try {
				// Setup
				socketClient 	= new Socket(HOST, PORT);
				reader 			= new BufferedReader(new InputStreamReader(socketClient.getInputStream()));
				writer 			= new DataOutputStream(socketClient.getOutputStream());
				socketClient.setSoTimeout(timeout);
				
				// Delay
				Thread.sleep(1000);

				// Leitura
				if ((msg = reader.readLine()) != null) {
					System.out.println("< Recebendo: " + msg);
					Thread.sleep(500);
				} msgToSend = processMsg(msg);				

				// Escrita
				if (msgToSend != null) {
					Thread.sleep(500);
					writeMsg(writer, msgToSend);
					Thread.sleep(500);
					writeMsg(writer, msgToSend);
				}

				// Delay
				Thread.sleep(1000);

				// Leitura
				if ((msg = reader.readLine()) != null) {
					System.out.println("< Recebendo: " + msg);
					Thread.sleep(500);
				} msgToSend = processMsg(msg);

				// Delay
				Thread.sleep(1000);
				
				// Encerra pra reabrir
				writer.close();
				reader.close();
				socketClient.close();
            } 
            catch (InterruptedException ie) {
				System.err.println(ie);
			} 
			catch (IOException e) {
				System.err.println(e);

			}
		}

    }

	/**
	 * @brief Método que envia mensagens para o veículo.
	 * 
	 * @param writer Variável responsável pela escrita (envio) das mensagens.
	 * @param msg Mensagem a ser enviada ao veículo.
	 */
	protected static void writeMsg(DataOutputStream writer, String msg) throws IOException {
		// writer.flush();
		System.out.println("> Enviando: " + msg);
		writer.writeUTF(msg);
	}

	/**
	 * @brief Método que processa mensagens advindas do veículo.
	 * 
	 * @param msg Mensagem enviada pelo veículo.
	 * 
	 * @return "V_1" Quando a mensagem recebida for válida. 
	 * @return null Quando não houve recebimento da mensagem.
	 */
	protected static String processMsg(String msg) {
		if (msg.equals("cheguei") || msg.equals(" cheguei")) {
			System.out.println("Bem-vindo ao estacionamento!");
			return "V_1";
		}
		
		return null;
	}
}