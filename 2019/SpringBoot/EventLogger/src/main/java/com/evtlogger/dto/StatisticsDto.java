package com.evtlogger.dto;

public class StatisticsDto {

    private String tgtName;
    private long totalIncome;
    private long totalOutgo;
    private String resultMsg;
    
    public String getTgtName() {
        return tgtName;
    }
    public void setTgtName(String tgtName) {
        this.tgtName = tgtName;
    }
    public long getTotalIncome() {
        return totalIncome;
    }
    public void setTotalIncome(long totalIncome) {
        this.totalIncome = totalIncome;
    }
    public long getTotalOutgo() {
        return totalOutgo;
    }
    public void setTotalOutgo(long totalOutgo) {
        this.totalOutgo = totalOutgo;
    }
    public String getResultMsg() {
        return resultMsg;
    }
    public void setResultMsg(String resultMsg) {
        this.resultMsg = resultMsg;
    }
    @Override
    public String toString() {
        return "tgtName:" + this.tgtName + ",totalIncome:" + this.totalIncome +
                ",totalOutgo:" + this.totalOutgo + ",resultMsg:" + this.resultMsg;
    }
}
