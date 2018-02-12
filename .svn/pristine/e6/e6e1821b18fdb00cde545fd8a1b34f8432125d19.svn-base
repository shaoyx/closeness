package com.graphtools.simrank;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileInputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.PriorityQueue;
import java.util.Set;
import java.util.regex.Pattern;

import org.apache.commons.cli.CommandLine;

import com.graphtools.GenericGraphTool;
import com.graphtools.utils.GraphAnalyticTool;
import com.graphtools.utils.Random;

@GraphAnalyticTool(
name = "SimRankGS",
description = "Compute SimRank based on graph sampling."
)
public class SimRankGS 
implements GenericGraphTool{
	protected static final Pattern SEPERATOR =  Pattern.compile("[\t ]");
	
	/* original id ==> new id */
	private HashMap<Integer, Integer> vertexMap = new HashMap<Integer, Integer>();
	private HashMap<Integer, Integer> rvertexMap = new HashMap<Integer, Integer>();
	/* use the new id and store the reversed graph. */
	private HashMap<Integer, ArrayList<Integer>> edgeList = new HashMap<Integer, ArrayList<Integer>>();
	
	private int sampleNum = 100;
	private int MAX_STEPS = 11;
	private int qv = -1;
	private int k;
	private double decayFactor = 0.6;
	
	private int [] sampledEdges;
	private int [] indegrees;
	private double [] simrankValues;
	
	public void topksimrank(){
		sampledEdges = new int[vertexMap.size()];
		simrankValues = new double[vertexMap.size()];
		indegrees = new int[vertexMap.size()];
		for(int vid = 0; vid < vertexMap.size(); vid++){
			simrankValues[vid] = 0;
		}
		simrankValues[qv] = -10;
		
		for(int i = 0; i < sampleNum; i++){
			/* 1. sample the graph */
			for(int vid = 0; vid < vertexMap.size(); vid++){
				indegrees[vid] = 0;
			}
			for(int vid = 0; vid < vertexMap.size(); vid++){
				ArrayList<Integer> nb = edgeList.get(vid);
				if(nb == null || nb.size() == 0){
					sampledEdges[vid] = -1;
				}
				else{
					sampledEdges[vid] = nb.get((int)(getRandomNextInt() % nb.size()));
					indegrees[sampledEdges[vid]]++;
				}
			}
			/* 2. compute the simrank */
			computePairWiseSimRank();
//			System.out.println("Sample "+i+":");
//			for(int vid = 0; vid < vertexMap.size(); vid++){
//				System.out.println("vid="+rvertexMap.get(vid)+" nb="+rvertexMap.get(sampledEdges[vid])+", smv="+simrankValues[vid]);
//			}
		}
		/* 3. get the top-k simrank */
		PriorityQueue<SimRankPair> pq = new PriorityQueue<SimRankPair>();
		for(int vid = 0; vid < vertexMap.size(); vid++){
			pq.add(new SimRankPair(vid, simrankValues[vid]));
			if(pq.size() > k)
				pq.remove(); // remove the smallest one
		}
		/* output the answer */
		int cnt = k;
//		System.out.println("(1300, " + String.format("%.9f", simrankValues[vertexMap.get(1300)] / sampleNum)+")");
//		System.out.println("(378, " + String.format("%.9f", simrankValues[vertexMap.get(378)] / sampleNum)+")");
//		System.out.println("(301, " + String.format("%.9f", simrankValues[vertexMap.get(301)] / sampleNum)+")");
		while(pq.isEmpty() == false){
			SimRankPair srp = pq.poll();
			System.out.println(cnt+" ==> ("+ rvertexMap.get(srp.getVertex())+", " + String.format("%.9f", srp.getSimRank() / sampleNum)+")");
			cnt--;
		}
	}
	
	private void computePairWiseSimRank() {
		HashMap<Integer, ArrayList<Integer>> visitedTimestamp = new HashMap<Integer, ArrayList<Integer>>();
		/* 1. calculate timestamp for the queryVertex. */
		int curVertex = qv;
		for(int s = 1; s <= MAX_STEPS; s++){
			int next = sampledEdges[curVertex];
			if(next == -1) break;
			if(visitedTimestamp.get(next) == null){
				visitedTimestamp.put(next, new ArrayList<Integer>());
			}
			visitedTimestamp.get(next).add(s);
			curVertex = next;
//			System.out.print(rvertexMap.get(next)+" ");
		}
//		System.out.println("");
		/* 2. compute each pairwise simrank. */
		double [] factor = new double[MAX_STEPS + 1];
		factor[0] = 1.0;
		for(int i = 1; i <= MAX_STEPS; i++){
			factor[i] = factor[i-1] * decayFactor;
		}

		int [] expanded = new int[vertexMap.size()];
		for(int v = 0; v < vertexMap.size(); v++){
			expanded[v] = -(1 << 30);
		}
		
		int arrayLen = MAX_STEPS + 1;
		int [] path = new int[arrayLen]; /* record the path. using cyclic array. */
		
		/* do the real computation*/
		for(int vid = 0; vid < vertexMap.size(); vid++){
			if(indegrees[vid] == 0 && vid != qv){
				/** NOTE: search one path and compute meet points. */
				/* initialization */
				int tail = 0;
				path[tail++] = vid;
				int lastUniqVertex = 0; /* The index of last vertex that hasn't be expanded. */
				int step = 0; /* current length of the path */
				expanded[vid] = 0;
				boolean isFull = false;

//				System.out.println("arrayLengh="+arrayLen+" tail="+tail);
				/* compute simrank */
				while(step <= lastUniqVertex + MAX_STEPS){
					int next = sampledEdges[path[((tail - 1) + arrayLen) % arrayLen]];
					if(next == -1) break;
					step++; // current lenght of the path.
					
					if(visitedTimestamp.get(next) != null){
						for(int stamp : visitedTimestamp.get(next)){
							int delta = tail - stamp;
							if(isFull == true && delta < 0) 
								delta += arrayLen; // MAX_STEPS + MAX_STEPS + 1 is the length of array.
							if(delta >= 0 && path[delta] != qv && (step - stamp) == expanded[path[delta]]){
//								if(path[delta] == vertexMap.get(6))
//									System.out.println("s="+step+" stamp="+stamp+" start="+vertexMap.get(vid)+" vid="+path[delta]+" delta="+factor[stamp]);
								simrankValues[path[delta]] += factor[stamp];
								expanded[path[delta]] *= -1; // make sure each vertex is only meet once.
							}
						}
					}
					
					/* maintain the last unique index. */
					if(expanded[next] == -(1 << 30)){
						expanded[next] = step;
						lastUniqVertex = step;
					}
					
					/* maintain the queue. */
					path[tail] = next; // record the path
					tail++;
					if(tail >= arrayLen){
						isFull = true;
						tail = 0;
					}
				}
//				for(int v = 0; v < vertexMap.size(); v++){
//					if(expanded[v] > 0)
//						expanded[v] *= -1;
//				}
			}
		}
	}

	private long getRandomNextInt(){
		long rand = Random.nextInt();
		return rand < 0 ? -rand : rand;
	}
	
	@Override
	public void run(CommandLine cmd) {
		if(cmd.hasOption("iter")){
			MAX_STEPS = Integer.valueOf(cmd.getOptionValue("iter"));
		}
		if(cmd.hasOption("sn")){
			sampleNum = Integer.valueOf(cmd.getOptionValue("sn"));
		}
		loadGraphFromAdj(cmd.getOptionValue("i"), Integer.valueOf(cmd.getOptionValue("qv")), Integer.valueOf(cmd.getOptionValue("qk")));
		topksimrank();
	}

	@Override
	public boolean verifyParameters(CommandLine cmd) {
		if(!cmd.hasOption("qv") || !cmd.hasOption("qk"))
			return false;
		return true;
	}

	private void loadGraphFromAdj(String graphFilePath, int qvertex, int k){
		int loop = 0;
		try {
			FileInputStream fin = new FileInputStream(graphFilePath);
			BufferedReader fbr = new BufferedReader(new InputStreamReader(fin));
			String line;
			int nid = 0;
			while((line = fbr.readLine()) != null){
				String [] values = SEPERATOR.split(line);
				int vid = Integer.valueOf(values[0]);
				if(vertexMap.get(vid) == null){
					vertexMap.put(vid, nid);
					rvertexMap.put(nid, vid);
					nid++;
				}
				vid = vertexMap.get(vid); // get the new id;
				
				for(int i = 1; i < values.length; ++i){
					int nvid = Integer.valueOf(values[i]);
					if(vertexMap.get(nvid) == null){
						vertexMap.put(nvid, nid);
						rvertexMap.put(nid, nvid);
						nid++;
					}
					nvid = vertexMap.get(nvid); // get the new id;
					
					if(nvid == vid){
						loop++;
						continue; //eliminate loop.
					}

					if(edgeList.get(nvid) == null){
						edgeList.put(nvid, new ArrayList<Integer>());
					}
					edgeList.get(nvid).add(vid); // reverse the edge: nvid --> vid
				}
			}
			fbr.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		// for debug
//		try {			
//			BufferedWriter bw = new BufferedWriter(new FileWriter("reversed.graph"));
//			
//			for(int vid : edgeList.keySet()){
//				bw.write(""+rvertexMap.get(vid));
//				ArrayList<Integer> neighbors = edgeList.get(vid);
//				for(int nb : neighbors){
//					bw.write(" "+rvertexMap.get(nb));
//				}
//				bw.newLine();
//			}
//			bw.close();
//		} catch (IOException e) {
//			e.printStackTrace();
//		}
		
		System.out.println("Query("+qvertex+","+k+")");
		this.k = k;
		this.qv = vertexMap.get(qvertex);
		
		System.out.println("reversed graph size="+edgeList.size()+" vsize="+vertexMap.size()+" loop="+loop);

	}	
}
