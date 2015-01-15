/*
 * HTTP requests can be sent to the server through a Java program. Below is a code snippet of an HTTP POST of an SCI request written in Java.
 *
 * Device Cloud's API Explorer can be used to run any web service request and export it as a Java application.
 */

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStreamWriter;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.Scanner;
/* 
 * Can replace this with any base 64 encoder for basic authentication. For java 6 installations on Sun's JRE you can use "sun.misc.BASE64Encoder" 
 * however this will not work in some installations (using OpenJDK). Java mail (javax.mail.internet.MimeUtility) also contains a Base 64 encoder in Java 6. 
 * A public domain version exists at http://iharder.sourceforge.net/current/java/base64/
 */
import org.apache.commons.codec.binary.Base64;
/**
* This is a stub class with a main method to run a Device Cloud web service.
*/
public class WebServiceRequest {
 /**
 * Run the web service request
 */
 public static void main(String[] args) { 
    try {
         // Create url to Device Cloud server for a given web service request
         URL url = new URL("http://login.etherios.com/ws/sci");
         HttpURLConnection conn = (HttpURLConnection)url.openConnection();
         conn.setDoOutput(true);
         conn.setRequestMethod("POST"); 
         // replace with your username/password
         String userpassword = "YourUsername:YourPassword"; 
         // can change this to use a different base64 encoder 
         String encodedAuthorization = Base64.encodeBase64String(userpassword.getBytes()).trim();
         conn.setRequestProperty("Authorization", "Basic "+ encodedAuthorization);
         // Send data to server
         conn.setRequestProperty("Content-Type", "text/xml");
         OutputStreamWriter out = new OutputStreamWriter(conn.getOutputStream());
         out.write("<sci_request version=\"1.0\"> \r\n");
         out.write(" <send_message> \r\n");
         out.write(" <targets> \r\n");
         out.write(" <device id=\"00000000-00000000-00000000-00000000\"/>\r\n");
         out.write(" </targets> \r\n");
         out.write(" <rci_request version=\"1.1\">\r\n");
         out.write(" <query_state/>\r\n");
         out.write(" </rci_request>\r\n");
         out.write(" </send_message>\r\n");
         out.write("</sci_request>\r\n");
         out.close();
         // Get input stream from response and convert to String
         conn.disconnect();
         conn.setDoInput(true);
         InputStream is = conn.getInputStream();
         Scanner isScanner = new Scanner(is);
         StringBuffer buf = new StringBuffer();
         while(isScanner.hasNextLine()) { 
            buf.append(isScanner.nextLine() +"\n");
         }
         // Output response to standard out
         String responseContent = buf.toString();
         System.out.println(responseContent);
    } catch (IOException e) { 
        // Print any exceptions that occur
        e.printStackTrace();
    }
 }
}