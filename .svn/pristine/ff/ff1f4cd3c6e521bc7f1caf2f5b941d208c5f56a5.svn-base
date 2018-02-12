package com.graphtools.simrank;

public class DegreeIdPair {
	
	private int degree;
	private int id;
	
	public DegreeIdPair(int degree, int id){
		this.degree = degree;
		this.id = id;
	}
	
	public int getDegree(){
		return degree;
	}
	
	public int getId(){
		return id;
	}
	
	public void decDegree(){
		degree--;
	}
	
	public void setDegree(int degree){
		this.degree = degree;
	}
	
	public void setId(int id){
		this.id = id;
	}
	
	public void copy(DegreeIdPair other){
		this.degree = other.degree;
		this.id = other.id;
	}
	
	public String toString(){
		return "("+ id+", "+degree+")";
	}
}
