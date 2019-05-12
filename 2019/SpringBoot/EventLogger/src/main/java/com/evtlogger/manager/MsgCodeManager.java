package com.evtlogger.manager;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.PropertySource;
import org.springframework.core.env.Environment;
import org.springframework.stereotype.Component;
import org.w3c.dom.Document;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

@Component
@PropertySource("classpath:config.properties")
public class MsgCodeManager extends AbsManager {
	// [Class private constants]
	private static final Logger logger = LoggerFactory.getLogger(MsgCodeManager.class);
	
	// [Class private variables]
	private Map<String, String> msgCodeMap = null;
	@Autowired private Environment env;
	
	// [Class public constants]
	public static final String SUCCESS_CODE = "00000";
	public static final String SYSTEM_FAIL_CODE = "99999";

	// [Methods]
	@Override // 매니저 클래스 초기화
	public boolean init() {
		// 에러코드와 메시지를 저장할 해시맵 생성
	    msgCodeMap = new HashMap<String, String>();
		
	    // 에러코드를 정의해둔 xml파일 열기
	    String xmlFileName = env.getProperty("msgCode.msgCodeXmlPath");
		File xmlFile = new File(xmlFileName);
		FileInputStream fis = null;
		
		if (!xmlFile.exists()) {
		    logger.error("The config file '" + xmlFileName + "' does NOT exist!");
		    return false;
		}

		// 파일 내용을 FileInputStream에 물림
		try {
            fis = new FileInputStream(xmlFile);
        }
		catch (FileNotFoundException e) {
            logger.error("Manager FileNotFoundException!", e);
            return false;
        }

        // XML 파서 생성 및 파싱 수행
	    DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();
        DocumentBuilder docBuilder = null;
        Document doc = null;
        boolean parseResult = false;
        
        try {
            docBuilder = dbFactory.newDocumentBuilder();
            doc = docBuilder.parse(fis);
            parseResult = true;
        }
        catch (ParserConfigurationException | SAXException | IOException | IllegalArgumentException e) {
            logger.error("Manager Exception!", e);
            parseResult = false;
        }
        finally {
            try {
                fis.close();
            }
            catch (IOException e) {
                logger.error("Manager IOException! (fis.close)", e);
                return false;
            }
            
            if (!parseResult) {
                return false;
            }
        }
        
        // 성공적으로 설정파일xml을 읽은 경우 <codes>안의 <code>태그들의 속성값들을 획득하여 msgCodeMap에 넣음
        // Lookup <codes>
        NodeList docNodeList = doc.getChildNodes();
        int docNodeListLen = docNodeList.getLength();

        for (int i = 0; i < docNodeListLen; ++i) {
            Node docNode = docNodeList.item(i);
            
            if (!docNode.getNodeName().equals("codes")) {
                continue;
            }
            
            // Lookup <code>
            NodeList codesNodeList = docNode.getChildNodes();
            int codesNodeListLen = codesNodeList.getLength();
            
            for (int j = 0; j < codesNodeListLen; ++j) {
                Node codesNode = codesNodeList.item(j);
                
                if (!codesNode.getNodeName().equals("code")) {
                    continue;
                }
                
                NamedNodeMap codeAttrMap = codesNode.getAttributes();
                String code = codeAttrMap.getNamedItem("code").getNodeValue();
                String msg = codeAttrMap.getNamedItem("msg").getNodeValue();
                
                msgCodeMap.put(code, msg);
            }
        }
	    
	    return managerInitialized = true;
	}
	
	// 코드에 해당하는 메시지 반환
	public String getMsg(String code) {
        // 매니저 초기화 여부 검사
        if (!managerInitialized) {
            if (!init()) {
                logger.error("Manager initialization failed!");
                return null;
            }
        }
        
		String msg = msgCodeMap.get(code);
		
		if (msg == null) {
			logger.error("미정의 메시지코드 '" + code + "'!");
			msg = msgCodeMap.get(SYSTEM_FAIL_CODE);
		}
		
		return msg;
	}
}
