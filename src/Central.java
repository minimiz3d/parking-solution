import java.io.*;
import java.net.*;
import java.nio.CharBuffer;
import java.util.*;

/*	Compilar: 					javac Central.java 
	Rodar: 						java -cp utilities.jar;. Central
*/

public class Central {
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

	private static void writeMsg(DataOutputStream writer, String msg) throws IOException {
		// writer.flush();
		System.out.println("> Enviando: " + msg);
		writer.writeUTF(msg);
	}

	private static String processMsg(String msg) {
		// Boolean teste = Arrays.asList(msg.split(" ")).contains("parking");

		if (msg.equals("cheguei") || msg.equals(" cheguei")) {
			System.out.println("Bem-vindo ao estacionamento!");
			return "V_1";
		}
		// else if (teste || msg.equals("+IPD,")){
		// 	System.out.println("Aguardando o veiculo estacionar...");
		// 	return "waiting";
		// }
		
		return null;
	}
}