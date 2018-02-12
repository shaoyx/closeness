package com.graphtools.partition;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileInputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.HashMap;
import java.util.regex.Pattern;

import org.apache.commons.cli.CommandLine;

import com.graphtools.GenericGraphTool;

public class BoundaryVertexCounter 
implements GenericGraphTool{

	protected static final Pattern SEPERATOR =  Pattern.compile("[\t ]");
	
	private HashMap<Integer, Integer> ps;
	private int partNum = -1;
	private int [] results;
	
	private void countBoundaryVertex(String graphFilePath, String savePath){
		this.countBoundaryVertexAdj(graphFilePath);
		this.saveResults(savePath);
	}
	
	private void countBoundaryVertexAdj(String graphFilePath){
		results = new int[partNum + 1]; 
		try {
			FileInputStream fin = new FileInputStream(graphFilePath);
			BufferedReader fbr = new BufferedReader(new InputStreamReader(fin));
			
			String line;
			while((line = fbr.readLine()) != null){
				String [] values = SEPERATOR.split(line);
				int vid = Integer.valueOf(values[0]);
				int pid = ps.get(vid);
				
				for(int i = 1; i < values.length; i++){
					int tmpPid = ps.get(Integer.valueOf(values[i]));
					if(tmpPid != pid){
						results[tmpPid]++;
					}
				}
			}
			fbr.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	private void countBoundaryVertexEdgelist(String graphFilePath){
		results = new int[partNum + 1]; 
		try {
			FileInputStream fin = new FileInputStream(graphFilePath);
			BufferedReader fbr = new BufferedReader(new InputStreamReader(fin));
			
			String line;
			while((line = fbr.readLine()) != null){
				String [] values = SEPERATOR.split(line);
				int vid = Integer.valueOf(values[0]);
				int pid = ps.get(vid);
				
				int tmpPid = ps.get(Integer.valueOf(values[1]));
				if(tmpPid != pid){
					results[tmpPid]++;
				}
			}
			fbr.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	private void saveResults(String savePath){
		try{
			BufferedWriter bw = new BufferedWriter(new FileWriter(savePath));
			int tot = 0;
			for(int i = 0; i <= partNum; ++i){
				bw.write(i+": "+results[i]);
				bw.newLine();
				tot += results[i];
			}
			bw.write("tot = "+tot);

			bw.close();
		}catch(IOException e){
			e.printStackTrace();
		}
		
	}
	
	private void loadPartitionSchema(String filePath){
		ps = new HashMap<Integer, Integer>();
		partNum = -1;
		try {
			FileInputStream fin = new FileInputStream(filePath);
			BufferedReader fbr = new BufferedReader(new InputStreamReader(fin));
			
			String line;
			while((line = fbr.readLine()) != null){
				String [] values = SEPERATOR.split(line);
				int vid = Integer.valueOf(values[0]);
				int pid = Integer.valueOf(values[1]);
				if(pid > partNum)
					partNum = pid;
				ps.put(vid, pid);
			}
			fbr.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	@Override
	public void run(CommandLine cmd) {
		loadPartitionSchema(cmd.getOptionValue("ps"));
		countBoundaryVertex(cmd.getOptionValue("i"), cmd.getOptionValue("sp"));
	}

	@Override
	public boolean verifyParameters(CommandLine cmd) {
		if(cmd.hasOption("ps") == false || cmd.hasOption("sp"))
			return false;
		return true;
	}

}
