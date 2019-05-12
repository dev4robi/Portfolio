package com.evtlogger;

import org.slf4j.MDC;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.EnableAspectJAutoProxy;

@EnableAspectJAutoProxy
@ComponentScan("com.evtlogger")
@SpringBootApplication
public class EventLoggerApplication {
    // [Methods]
	public static void main(String[] args) {
		MDC.put("layer", "SYS");
		SpringApplication.run(EventLoggerApplication.class, args);
	}
}