package com.graphtools.ktruss;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Set;
import java.util.regex.Pattern;

import org.apache.commons.cli.CommandLine;

import com.graphtools.GenericGraphTool;
import com.graphtools.utils.GraphAnalyticTool;

@GraphAnalyticTool(
name = "ktruss routien",
description = "ktruss related routien: local algorithm, verification"
)
public class KTruss 
implements GenericGraphTool{

	protected static final Pattern SEPERATOR =  Pattern.compile("[\t ]");
	
	boolean isVerification = false;
	
	Set<Integer> vertexSet = new HashSet<Integer>();
	HashMap<Integer, Set<Integer>> edgeList = new HashMap<Integer, Set<Integer>>();
	
	private void loadGraphFromAdj(String graphFilePath){
		try {
			FileInputStream fin = new FileInputStream(graphFilePath);
			BufferedReader fbr = new BufferedReader(new InputStreamReader(fin));
			String line;
			while((line = fbr.readLine()) != null){
				String [] values = SEPERATOR.split(line);
				int vid = Integer.valueOf(values[0]);
				vertexSet.add(vid);
				if(edgeList.get(vid) == null){
					edgeList.put(vid, new HashSet<Integer>());
				}
				
				for(int i = 1; i < values.length; ++i){
					int nvid = Integer.valueOf(values[i]);
					vertexSet.add(nvid);

					if(edgeList.get(nvid) == null){
						edgeList.put(nvid, new HashSet<Integer>());
					}
					
					edgeList.get(vid).add(nvid);
					edgeList.get(nvid).add(vid);
				}
			}
			fbr.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	
	@Override
	public void run(CommandLine cmd) {
		loadGraphFromAdj(cmd.getOptionValue("i"));
		if(this.isVerification){
			verification(Integer.valueOf(cmd.getOptionValue("th")));
		}
		else{
			System.out.println("Not Support Yet!");
		}
	}
	
	public void verification(int threshold){
		for(int vid : vertexSet){
			if(edgeList.get(vid).size() < threshold - 1){
				System.out.println("Invalid vertex: "+vid+
						", it has degree ="+edgeList.get(vid));
				continue;
			}
			for(int nvid1 : edgeList.get(vid)){
				int count = 0;
				for(int nvid2 : edgeList.get(nvid1)){
					if(nvid2 != vid && edgeList.get(vid).contains(nvid2)){
						count++;
					}
				}

				if(count < threshold - 2){
					System.out.println("Invalid: ("+vid+","+nvid1+")");
				}
			}
		}
	}

	@Override
	public boolean verifyParameters(CommandLine cmd) {
		if(!cmd.hasOption("th"))
			return false;
		if(cmd.hasOption("t") && "verification".equals(cmd.getOptionValue("t"))){
			isVerification = true;
		}
		return true;
	}

}
