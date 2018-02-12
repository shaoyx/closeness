package com.graphtools;

import java.util.List;
import java.util.Scanner;

import org.apache.commons.cli.BasicParser;
import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.CommandLineParser;
import org.apache.commons.cli.HelpFormatter;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.ParseException;

import com.graphtools.subgraphmatch.DegreeCounter;
import com.graphtools.subgraphmatch.EdgeIndexBuilder;
import com.graphtools.subgraphmatch.NeighborLabelExtractor;
import com.graphtools.subgraphmatch.QueryEnumerator;
import com.graphtools.subgraphmatch.QuerySymmetryBroker;
import com.graphtools.subgraphmatch.RandomQueryGenerator;
import com.graphtools.subgraphmatch.RelabelByDegree;
import com.graphtools.utils.AnnotationUtils;
import com.graphtools.utils.GraphAnalyticTool;

public class GraphTools {

	private static Options OPTIONS;
	  static {
		    OPTIONS = new Options();
		    OPTIONS.addOption("h", "help", false, "Help");
		    OPTIONS.addOption("lt", "listTools", false, "List supported tools");
		    OPTIONS.addOption("tc", "toolClass", true, "Specifiy the tool class");
		    OPTIONS.addOption("i", "input", true, "Path of the input graph");
		    
		    /** Bahmani Method **/
		    OPTIONS.addOption("e", "error", true, "The error rate for the greedy approximation algortihm by Bahmani.");
		    
		    /** partition Method **/
		    OPTIONS.addOption("k", "ksize", true, "Size of the densest subgraph");
		    
		    /** relabel graph by degree order */
		    OPTIONS.addOption("lp","labelPath", true, "the path of the new label file");
		    OPTIONS.addOption("sp", "savePath", true, "the path of the save file");
		    
		    /** degree counter **/
		    OPTIONS.addOption("dp", "degreePath", true, "the path of the degree file");
		    
		    /** k truss */
		    OPTIONS.addOption("th", "threshold", true, "threshold for the ktruss");
		    OPTIONS.addOption("t", "type", true, "verification or execution");
		    
		    /** edge index build*/
		    OPTIONS.addOption("np", "npart", true, "number of partition. [default: 1]");
		    OPTIONS.addOption("nh", "nhash", true, "number of hash function. [default: 8]");
		    OPTIONS.addOption("ne", "nedge", true, "number of edges to be indexed");
		    OPTIONS.addOption("f", "factor", true, "factor. [default: 16]");
		    OPTIONS.addOption("ht", "htype", true, "hash type. [default: 0 (0: JenkinsHash; 1: MurmurHash)]");
		    
		    /** partition statistic */
		    OPTIONS.addOption("ps", "pscheme", true, "the file path of partition scheme");
		    
		    /** LSimRank parameters */
		    OPTIONS.addOption("qv","qvertex", true, "the vertex id for top-k simrank query");
		    OPTIONS.addOption("qk", "qk", true, "the k for top-k simrank query");
		    OPTIONS.addOption("iter", "iteration", true, "the maximal number of steps");
		    OPTIONS.addOption("sn", "sampleNum", true, "the number of sample (Monte Carlo Simuration)");
		    OPTIONS.addOption("qsn", "querySampleNum", true, "the number of sample for the query's random walk (Monte Carlo Simuration)");
		    OPTIONS.addOption("m", "method", true, "specify the method for computing the SimRank.");
		  }
	

	private static void run(CommandLine cmd) throws InstantiationException, IllegalAccessException, ClassNotFoundException {
		if(cmd.hasOption("tc") == false || cmd.hasOption("i") == false){
			printHelp();
			return;
		}
		
		String className = cmd.getOptionValue("tc");
//		System.out.println(cmd.getOptionValue("tc"));
//		System.out.println(cmd.getOptionValue("i"));
//		System.out.println("ClassName="+className);
		GenericGraphTool graphTool = (GenericGraphTool) Class.forName(className).newInstance();
		
		if(graphTool.verifyParameters(cmd) == false){
			printHelp();
			return;
		}
		long startTime = System.currentTimeMillis();
		graphTool.run(cmd);
		System.out.println("Runtime: "+ (System.currentTimeMillis() - startTime)+" ms");
	}

	private static void printHelp() {
	    HelpFormatter formatter = new HelpFormatter();
	    formatter.printHelp(GraphTools.class.getName(), OPTIONS, true);
		
	}

	private static void listTools() { 
		List<Class<?>> classes = AnnotationUtils.getAnnotatedClasses(
		      GraphAnalyticTool.class, "com.graphtools");
		    System.out.print("  Supported tools:\n");
		    for (Class<?> clazz : classes) {
		    	GraphAnalyticTool tool = clazz.getAnnotation(GraphAnalyticTool.class);
		        StringBuilder sb = new StringBuilder();
		        sb.append(tool.name()).append(" - ").append(clazz.getName())
		            .append("\n");
		        if (!tool.description().equals("")) {
		          sb.append("    ").append(tool.description()).append("\n");
		        }
		        System.out.print(sb.toString());
		    }
	}

	public static void main(String[] args) throws Exception{
		
		/* 1. parse the args */
	    CommandLineParser parser = new BasicParser();
		CommandLine cmd = parser.parse(OPTIONS, args);
		
		if(cmd.hasOption("h")){
			printHelp();
			return ;
		}
		
		if(cmd.hasOption("lt")){
			listTools();
			return ;
		}

		/* 2. run the proper tool */
		run(cmd);
	}
}
