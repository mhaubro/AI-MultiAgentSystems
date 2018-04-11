package searchclient;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.LinkedList;

import searchclient.Memory;
import searchclient.Strategy.*;
import searchclient.Heuristic.*;

public class SearchClient {
	public Node initialState;


    public SearchClient(BufferedReader serverMessages) throws Exception {
		// Read lines specifying colors
		String line = serverMessages.readLine();
		if (line.matches("^[a-z]+:\\s*[0-9A-Z](\\s*,\\s*[0-9A-Z])*\\s*$"))
		{
			System.err.println("Error, client does not support colors.");
			System.exit(1);
		}

		int row = 0;
		boolean agentFound = false;
		this.initialState = new Node(null);
		// For loading use arraylist as size is not yet known.
		ArrayList<Box> boxes = new ArrayList<>();
		ArrayList<Agent> agents = new ArrayList<>();
		ArrayList<Goal> goals = new ArrayList<>();

		while (!line.equals(""))
		{
			for (int col = 0; col < line.length(); col++)
			{
				char chr = line.charAt(col);

				if(line.length() > Node.MAX_COL)
				    Node.MAX_COL = line.length();

				if (chr == '+')
				{ // Wall.
					Node.walls[row][col] = true;
				}
				else if ('0' <= chr && chr <= '9')
				{ // Agent.
					agents.add(new Agent((int) chr, new Pair<>(row, col)));
				}
				else if ('A' <= chr && chr <= 'Z')
				{
					// Box.
					boxes.add(new Box(chr, new Pair<>(row, col)));
				}
				else if ('a' <= chr && chr <= 'z')
				{
					// Goal.
					goals.add(new Goal(chr, new Pair<>(row, col)));
				}
				else if (chr == ' ')
				{
					// Free space.
				}
				else
				{
					System.err.println("Error, read invalid level character: " + (int) chr);
					System.exit(1);
				}
			}
			line = serverMessages.readLine();
			row++;
		}
		Node.MAX_ROW = row;
		// Arraylists to arrays
		Node.goals = new Goal[goals.size()];
		Node.goals = goals.toArray(Node.goals);
		this.initialState.boxes = new Box[boxes.size()];
		this.initialState.boxes = boxes.toArray(this.initialState.boxes);
		this.initialState.agents = new Agent[agents.size()];
		this.initialState.agents = agents.toArray(this.initialState.agents);
	}

	public LinkedList<Node> Search(Strategy strategy) throws IOException {
		System.err.format("Search starting with strategy %s.\n", strategy.toString());
		strategy.addToFrontier(this.initialState);

		int iterations = 0;
		while (true) {
            if (iterations == 1000) {
				System.err.println(strategy.searchStatus());
				iterations = 0;
			}

			if (strategy.frontierIsEmpty()) {
				return null;
			}

			Node leafNode = strategy.getAndRemoveLeaf();

			

			if (leafNode.isGoalState()) {
				return leafNode.extractPlan();
			}

			strategy.addToExplored(leafNode);
			//System.err.println("Hashcode of agent: " + String.valueOf(leafNode.hashCode()));
			//System.err.println("Hej: " + leafNode.isGoalState());
			for (Node n : leafNode.getExpandedNodes()) { // The list of expanded nodes is shuffled randomly; see Node.java.
				if (!strategy.isExplored(n) && !strategy.inFrontier(n)) {
					strategy.addToFrontier(n);
				}
			}
			iterations++;
		}
	}

	public static void main(String[] args) throws Exception {
		BufferedReader serverMessages = new BufferedReader(new InputStreamReader(System.in));

		// Use stderr to print to console
		System.err.println("SearchClient initializing. I am sending this using the error output stream.");

		// Read level and create the initial state of the problem
		SearchClient client = new SearchClient(serverMessages);

        Strategy strategy;
        if (args.length > 0) {
            switch (args[0].toLowerCase()) {
                case "-bfs":
                    strategy = new StrategyBFS();
                    break;
                case "-dfs":
                    strategy = new StrategyDFS();
                    break;
                case "-astar":
                    strategy = new StrategyBestFirst(new AStar(client.initialState));
                    break;
                case "-wastar":
                    // You're welcome to test WA* out with different values, but for the report you must at least indicate benchmarks for W = 5.
                    strategy = new StrategyBestFirst(new WeightedAStar(client.initialState, 5));
                    break;
                case "-greedy":
                    strategy = new StrategyBestFirst(new Greedy(client.initialState));
                    break;
                default:
                    strategy = new StrategyBFS();
                    System.err.println("Defaulting to BFS search. Use arguments -bfs, -dfs, -astar, -wastar, or -greedy to set the search strategy.");
            }
        } else {
            strategy = new StrategyBFS();
            System.err.println("Defaulting to BFS search. Use arguments -bfs, -dfs, -astar, -wastar, or -greedy to set the search strategy.");
        }

		LinkedList<Node> solution;
		try {
			solution = client.Search(strategy);
		} catch (OutOfMemoryError ex) {
			System.err.println("Maximum memory usage exceeded.");
			solution = null;
		}

		if (solution == null) {
			System.err.println(strategy.searchStatus());
			System.err.println("Unable to solve level.");
			System.exit(0);
		} else {
			System.err.println("\nSummary for " + strategy.toString());
			System.err.println("Found solution of length " + solution.size());
			System.err.println(strategy.searchStatus());

			for (Node n : solution) {
				String act = n.action.toString();
				System.out.println(act);
				String response = serverMessages.readLine();
				if (response.contains("false")) {
					System.err.format("Server responsed with %s to the inapplicable action: %s\n", response, act);
					System.err.format("%s was attempted in \n%s\n", act, n.toString());
					break;
				}
			}
		}
	}
}
