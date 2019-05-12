package com.evtlogger.controller.statistics;

import java.util.Map;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestHeader;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.servlet.ModelAndView;

import com.evtlogger.etc.ServiceReturn;
import com.evtlogger.service.statistics.IStatisticsService;

@RestController
public class StatisticsRestController {
    // [Class private constants]
    private static final Logger logger = LoggerFactory.getLogger(StatisticsRestController.class);
    
    // [Class private variables]
    @Autowired private IStatisticsService statisticsSvc;

    @GetMapping("/statistics")
    public ModelAndView statisticsUI() {
        return new ModelAndView("statistics");
    }
    
    @GetMapping("/statistics/all")
    public Map<String, Object> statisticAll(@RequestHeader(required=true) String userJwt) {
        ServiceReturn svcReturn = statisticsSvc.statisticAll(userJwt, null);
        return (svcReturn == null ? null : svcReturn.toMap());
    }
    
    @GetMapping("/statistics/name/{tgtName}")
    public Map<String, Object> staticticByName(
            @RequestHeader(required=true) String userJwt,
            @PathVariable("tgtName") String tgtName
    ) {
        ServiceReturn svcReturn = statisticsSvc.statisticByName(userJwt, null, tgtName);
        return (svcReturn == null ? null : svcReturn.toMap());
    }
    
    @GetMapping("statistics/phone/{tgtPhoneNum}")
    public Map<String, Object> statisticByPhoneNum(
            @RequestHeader(required=true) String userJwt,
            @PathVariable("tgtPhoneNum") String tgtPhoneNum
    ) {
        ServiceReturn svcReturn = statisticsSvc.statisticByName(userJwt, null, tgtPhoneNum);
        return (svcReturn == null ? null : svcReturn.toMap());
    }
}
