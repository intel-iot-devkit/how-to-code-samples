import java.io.IOException;
import java.util.ArrayList;
import java.util.Timer;
import java.util.TimerTask;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

public class ColorMatchGame {

	private static LcdScreen lcdScreen = new LcdScreen();
	private static ArrayList<String> colorSequence = new ArrayList<String>();
	private static int checkedIndex = 0;
	private static int lastColorNum = 1;
	public static final String BLUE = "blue";
	public static final String RED = "red";
	public static final String GREEN = "green";
	public static final String YELLOW = "yellow";
	public static final String WHITE = "white";

	public static void main(String[] args) {
		lcdScreen.setLcdColor("white");
		setupServer();
	}

	/**
	 * defines web server endpoints and defines a method to run for each
	 */
	private static void setupServer(){
		ServerSetup server = new ServerSetup();
		server.setupServer(8080);
		server.addServlet("/" +BLUE, new ServerSetup.GetCall() { //user clicked blue

			@Override
			public void runCall(HttpServletRequest request, HttpServletResponse response)
					throws IOException {
				// TODO Auto-generated method stub
				handleUserInput(BLUE,response);
			}
		});
		server.addServlet("/" +GREEN, new ServerSetup.GetCall() { //user clicked green

			@Override
			public void runCall(HttpServletRequest request, HttpServletResponse response)
					throws IOException {
				// TODO Auto-generated method stub
				handleUserInput(GREEN,response);
			}
		});
		server.addServlet("/" +RED, new ServerSetup.GetCall() { //user clicked red

			@Override
			public void runCall(HttpServletRequest request, HttpServletResponse response)
					throws IOException {
				// TODO Auto-generated method stub
				handleUserInput(RED,response);
			}
		});
		server.addServlet("/" +YELLOW, new ServerSetup.GetCall() { //user clicked yellow

			@Override
			public void runCall(HttpServletRequest request, HttpServletResponse response)
					throws IOException {
				// TODO Auto-generated method stub
				handleUserInput(YELLOW,response);
			}
		});
		server.addServlet("/nextLevel", new ServerSetup.GetCall() { //level ended

			@Override
			public void runCall(HttpServletRequest request, HttpServletResponse response)
					throws IOException {
				// TODO Auto-generated method stub
				startNextLevel();
				response.setStatus(200);
			}
		});
		server.addServlet("/startGame", new ServerSetup.GetCall() {//game started

			@Override
			public void runCall(HttpServletRequest request, HttpServletResponse response)
					throws IOException {
				// TODO Auto-generated method stub
				startGame();
			}
		});
		server.run();
	}
	
	/**
	 * initiates game
	 */
	private static void startGame(){
		checkedIndex = 0;
		colorSequence.clear();
		addRandomColorToSequence();
		playFullSequenceOfColors();
	}
	
	/**
	 * prepares and starts next level
	 */
	private static void startNextLevel(){
		checkedIndex = 0;
		addRandomColorToSequence();
		playFullSequenceOfColors();
	}

	/**
	 * deals with a user picking a color
	 * @param colorInput
	 * @param response
	 */
	private static void handleUserInput(String colorInput, HttpServletResponse response){
		lcdScreen.setLcdColor(colorInput);
		if(!checkUserColorInput(colorInput)){
			try {
				response.getWriter().write("error");
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			return;
		};
		if(checkLevelEnd()){
			try {
				response.getWriter().write("nextLevel");
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			return;
		}
		try {
			response.getWriter().write("ok");
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	/**
	 * adds another color to the sequence
	 */
	private static void addRandomColorToSequence(){
		int randomColor = (int) Math.floor(Math.random()*4 +1);
		while(randomColor == lastColorNum){
			randomColor = (int) Math.floor(Math.random()*4 +1);
		}
		lastColorNum = randomColor;
		switch(randomColor){
		case 1:
			colorSequence.add(BLUE);
			break;
		case 2:
			colorSequence.add(RED);
			break;
		case 3:
			colorSequence.add(GREEN);
			break;
		case 4:
			colorSequence.add(YELLOW);
			break;
		}
	}
	
	/**
	 * checks if the levels ended
	 * @return boolean has the level ended
	 */
	private static boolean checkLevelEnd(){
		boolean isLevelEnd = checkedIndex == colorSequence.size();
		if(isLevelEnd){
			lcdScreen.setLcdColor(WHITE);
		}
		return isLevelEnd;
	}

	/**
	 * plays sequence of colors in sequence
	 */
	private static void playFullSequenceOfColors(){
		int count = 1;
		colorSequence.add(WHITE);
		for(String color: colorSequence){
			int timeToWaitBeforeShowingColor = 1000 * 2 * count;
			Timer timer = new Timer();
			TimerTask timerTask = null;

			switch(color){

			case BLUE:
				timerTask = new TimerTask() {
					@Override
					public void run() {
						// TODO Auto-generated method stub
						lcdScreen.setLcdColor(BLUE);
					}
				};
				break;
			case GREEN:
				timerTask = new TimerTask() {

					@Override
					public void run() {
						// TODO Auto-generated method stub
						lcdScreen.setLcdColor(GREEN);
					}
				};
				break;
			case RED:
				timerTask = new TimerTask() {
					@Override
					public void run() {
						// TODO Auto-generated method stub
						lcdScreen.setLcdColor(RED);
					}
				};
				break;
			case YELLOW:
				timerTask = new TimerTask() {

					@Override
					public void run() {
						// TODO Auto-generated method stub
						lcdScreen.setLcdColor(YELLOW);
					}
				};
				break;
			case WHITE:
				timerTask = new TimerTask() {
					@Override
					public void run() {
						// TODO Auto-generated method stub
						colorSequence.remove(colorSequence.size()-1);
						lcdScreen.setLcdColor(WHITE);
					}
				};
				break;
			}
			timer.schedule(timerTask, timeToWaitBeforeShowingColor);
			count++;
		}
	}

	public static boolean checkUserColorInput(String color){
		boolean answer = colorSequence.get(checkedIndex).equals(color);
		checkedIndex++;
		if(!answer){
			lcdScreen.setLcdColor(WHITE);
		}
		return answer;
	}
}
