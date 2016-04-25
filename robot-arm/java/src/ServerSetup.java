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

public class ServerSetup {
	private Server server;
	private ServletContextHandler context0;

	/**
	 * setup server configurations
	 * @param port to connect server to
	 */
	public void setupServer(int port){
		server = new Server(port);
		context0 = new ServletContextHandler(ServletContextHandler.SESSIONS);
		context0.setContextPath("/");
	}


	/**
	 * add servlet with post call
	 * @param path- path to reach to
	 * @param postCall function to run
	 */
	public void addServletWithPostCall(String path, PostHttpCall postCall){
		PostCallServlet postServlet = new PostCallServlet(postCall);
		ServletHolder servletHolder = new ServletHolder(postServlet);
		context0.addServlet(servletHolder,path);
	}

	/**
	 * add servlet with get call
	 * @param path- path to reach to
	 * @param getCall function to run
	 */
	public void addServletWithGetCall(String path, GetHttpCall getCall){
		GetCallServlet getServlet = new GetCallServlet(getCall);
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
			//server.join();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * class for storing the running function of the servlet for the case of a post request
	 *
	 */
	@SuppressWarnings("serial")
	public class PostCallServlet extends HttpServlet
	{
		private PostHttpCall postCall;
		public PostCallServlet(PostHttpCall postCallParam){
			postCall = postCallParam;
		}

		@Override
		protected void doPost( HttpServletRequest request,
				HttpServletResponse response ) throws ServletException,
				IOException
		{
			postCall.runCall(request, response);
		}
	}

	public static interface PostHttpCall {
		public void runCall(HttpServletRequest request,
				HttpServletResponse response) throws IOException;
	}


	/**
	 * class for storing the running function of the servlet for the case of a get request
	 *
	 */
	@SuppressWarnings("serial")
	public class GetCallServlet extends HttpServlet
	{
		private GetHttpCall getCall;
		public GetCallServlet(GetHttpCall getCallParam){
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

	public static interface GetHttpCall {
		public void runCall(HttpServletRequest request,
				HttpServletResponse response) throws IOException;
	}
}
