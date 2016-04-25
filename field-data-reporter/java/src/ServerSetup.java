import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.eclipse.jetty.server.Handler;
import org.eclipse.jetty.server.Server;
import org.eclipse.jetty.server.handler.ContextHandlerCollection;
import org.eclipse.jetty.servlet.ServletContextHandler;
import org.eclipse.jetty.servlet.ServletHolder;

/**
 * 
 * @author rhassidi
 *
 */
public class ServerSetup {
    private Server server;
    private ServletContextHandler context0;

    /**
     * setup server configurations
     */
    public void setupServer(int port){
	server = new Server(port);
	context0 = new ServletContextHandler(ServletContextHandler.SESSIONS);
	context0.setContextPath("/");
    }

    /**
     * add servlet
     * @param path- path to reach to
     * @param getCall function to tun
     */
    public void addServlet(String path, GetCall getCall){
	GetServlet getServlet = new GetServlet(getCall);
	ServletHolder servletHolder = new ServletHolder(getServlet);
	context0.addServlet(servletHolder,path);
    }

    /**
     * starts the server 
     */
    public void run(){
	ContextHandlerCollection contexts = new ContextHandlerCollection();
	contexts.setHandlers(new Handler[]{context0});
	server.setHandler(contexts);
	try {
	    server.start();
	    server.join();
	} catch (Exception e) {
	    e.printStackTrace();
	}
    }


    /**
     * class for storing the running function of the servlet
     *
     */
    @SuppressWarnings("serial")
    public class GetServlet extends HttpServlet{
	private GetCall getCall;
	
	public GetServlet(GetCall getCallParam){
	    getCall = getCallParam;
	}

	@Override
	protected void doGet( HttpServletRequest request,
		HttpServletResponse response ) throws ServletException,
		IOException
	{
	    getCall.runCall(request, response);
	}
    }

    public static interface GetCall {
	public void runCall(HttpServletRequest request,
		HttpServletResponse response) throws IOException;
    }

}
