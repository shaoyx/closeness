package com.graphtools.simrank;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.regex.Pattern;

import org.apache.commons.cli.CommandLine;

import com.graphtools.GenericGraphTool;
import com.graphtools.utils.GraphAnalyticTool;

@GraphAnalyticTool(
name = "SimRankUniverse",
description = "An entry to call various simrank computation methods:" +
		"LSimRank, SimRankWWW05, SimRankMOD14, SimRankGS."
)
/**
 * An entry for calling various simrank methods which are all based on 
 * random surfer model (first-meeting and non-first-meeting).
 * It centralized loads graph data and sets common configuration here.
 * @author ggbond
 *
 */
public class SimRankUniverse 
implements GenericGraphTool{
	protected static final Pattern SEPERATOR =  Pattern.compile("[\t ]");

	/* original id ==> new id */
	public HashMap<Integer, Integer> vertexMap = new HashMap<Integer, Integer>();
	/* new id ==> original id */
	public HashMap<Integer, Integer> rvertexMap = new HashMap<Integer, Integer>();
	/* use the new id and store the reversed graph. */
	public HashMap<Integer, ArrayList<Integer>> edgeList = new HashMap<Integer, ArrayList<Integer>>();
	
	/* configuration */
	public int sampleNum = 100;
	public int querySampleNum = 100;
	public int MAX_STEPS = 11;
	public int qv = -1;
	public int k;
	public double decayFactor = 0.6;
	private String method = "all";
	
	@Override
	public void run(CommandLine cmd) {
		if(cmd.hasOption("iter")){
			MAX_STEPS = Integer.valueOf(cmd.getOptionValue("iter"));
		}
		if(cmd.hasOption("sn")){
			sampleNum = Integer.valueOf(cmd.getOptionValue("sn"));
		}
		if(cmd.hasOption("qsn")){
			querySampleNum = Integer.valueOf(cmd.getOptionValue("qsn"));
		}
		if(cmd.hasOption("m")){
			method = cmd.getOptionValue("m");
		}

		loadGraphFromAdj(cmd.getOptionValue("i"), Integer.valueOf(cmd.getOptionValue("qv")), Integer.valueOf(cmd.getOptionValue("qk")));
		
		long startTime = System.currentTimeMillis();
		/* call the method based on the input parameters. */
		if("LSimRank".equals(method) || "all".equals(method)){
			startTime = System.currentTimeMillis();
			new LSimRank(this).topkcomputation();
			System.out.println("LSimRank Runtime: "+ (System.currentTimeMillis() - startTime)+" ms");
		}
		else if("SimRankWWW05".equals(method) || "all".equals(method)){
			startTime = System.currentTimeMillis();
			new SimRankWWW05(this).topksimrank();
			System.out.println("SimRankGS Runtime: "+ (System.currentTimeMillis() - startTime)+" ms");
		}
		else if("SimRankMOD14".equals(method) || "all".equals(method)){
			startTime = System.currentTimeMillis();
			new SimRankMOD14(this).topksimrank();
			System.out.println("SimRankGS Runtime: "+ (System.currentTimeMillis() - startTime)+" ms");
		}
		else if("SimRankGS".equals(method) || "all".equals(method)){
			startTime = System.currentTimeMillis();
			new SimRankGSasIndex(this).topksimrank();
			System.out.println("SimRankGS Runtime: "+ (System.currentTimeMillis() - startTime)+" ms");
		}
		else{
			System.out.println("Unsupported Method: "+ method);
		}
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
		
		System.out.println("reversed graph size="+edgeList.size()+" vsize="+vertexMap.size()+" delete loop="+loop);

	}	
}
