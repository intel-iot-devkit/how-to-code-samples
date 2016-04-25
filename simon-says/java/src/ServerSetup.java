import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.eclipse.jetty.server.Server;
import org.eclipse.jetty.server.ServerConnector;
import org.eclipse.jetty.servlet.DefaultServlet;
import org.eclipse.jetty.servlet.ServletContextHandler;
import org.eclipse.jetty.servlet.ServletHolder;

public class ServerSetup {
	private Server server;
	private ServletContextHandler context;
	ServletHolder holderPwd = new ServletHolder("default", DefaultServlet.class);

	/**
	 * setup server configurations
	 * @param port to connect server to
	 */
	public void setupServer(int port){
		server = new Server(port);
		ServerConnector connector = new ServerConnector(server);
		server.addConnector(connector);

		//adding default servlet at "/" according to servlet spec
		context = new ServletContextHandler(ServletContextHandler.SESSIONS);
		context.setResourceBase("/var/SimonSays/");
		context.setContextPath("/");
		server.setHandler(context);

		// add special path spec of "/home/" content mapped to the SimonSays folder
		ServletHolder holderHome = new ServletHolder("static-home", DefaultServlet.class);
		holderPwd.setInitParameter("resourceBase","/var/SimonSays/");
		holderHome.setInitParameter("dirAllowed","true");
		holderHome.setInitParameter("pathInfoOnly","true");
		context.addServlet(holderHome,"/home/*");
	}


	/**
	 * add servlet
	 * @param path- path to reach to
	 * @param getCall function to tun
	 */
	public void addServlet(String path, GetCall getCall){
		System.out.println("add servelet");
		GetServlet getServlet = new GetServlet(getCall);
		ServletHolder servletHolder = new ServletHolder("dynamic" + "-" + path,getServlet);
		context.addServlet(servletHolder,path);
	}

	/**
	 * starts the server 
	 */
	public void run(){
		try {
			server.start();
			server.dump(System.err);
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
	public class GetServlet extends HttpServlet
	{
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
