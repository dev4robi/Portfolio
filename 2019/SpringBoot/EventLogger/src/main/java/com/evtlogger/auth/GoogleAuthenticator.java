package com.evtlogger.auth;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.nio.charset.Charset;
import java.security.GeneralSecurityException;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.core.env.Environment;
import org.springframework.stereotype.Component;

import com.google.api.client.googleapis.auth.oauth2.GoogleIdToken;
import com.google.api.client.googleapis.auth.oauth2.GoogleIdToken.Payload;
import com.google.api.client.googleapis.auth.oauth2.GoogleIdTokenVerifier;
import com.google.api.client.http.javanet.NetHttpTransport;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.google.api.client.json.jackson2.JacksonFactory;

@Component
public class GoogleAuthenticator implements IAuthenticator {
	// [References]
	// https://developers.google.com/identity/sign-in/android/backend-auth
	
	// [Class private constants]
	private static final Logger logger = LoggerFactory.getLogger(GoogleAuthenticator.class);
	private static final JacksonFactory _JacsonFactory = new JacksonFactory();
	private static final NetHttpTransport _HttpTransport = new NetHttpTransport();

	// [Class private variables]
	@Autowired private Environment _env;

	// [Methods]
	@Override // 구글 OAuth2.0
	public Map<String, Object> authenticate(String idToken) {
		GoogleIdTokenVerifier verifier = null;
		GoogleIdToken googleIdToken = null;
		Map<String, Object> rtMap = null;
		
		// 1. Google API
		try {
			verifier = new GoogleIdTokenVerifier.Builder(_HttpTransport, _JacsonFactory)
				.setAudience(Collections.singletonList(_env.getProperty("auth.google.url")))
				.build();
			googleIdToken = verifier.verify(idToken);
		}
		catch (GeneralSecurityException | IOException e) {
			logger.error("Exception!", e);
		}
	
		if (googleIdToken != null) {
			// 구글 계정정보 획득
			Payload payload = googleIdToken.getPayload();
			rtMap = new HashMap<String, Object>();
			
			for (String key : payload.keySet()) {
				Object val = payload.get(key);
				rtMap.put(key, val);
				logger.debug("key: " + key + ", val: " + val); // [###]debug
			}
		}
		else {
			logger.error("'googleIdToken' is null! Now trying to call custom function...");
		}

		// 2. Custom API
		if (rtMap == null) {
			if ((rtMap = getMapFromGoogleIdToken(idToken)) == null) {
				logger.error("Google-OAuth custom fucntion failed!");
				return null;
			}			
		}
		
		return rtMap;
	}
	
	// Google-OAuth2.0 Custom function 
	private Map<String, Object> getMapFromGoogleIdToken(final String idToken) {
		Map<String, Object> rtMap = null;
		BufferedReader br = null;
		
		// URL Connection
		try {
			// Get information from token by contacting the google_token_verify_tool url
			br = new BufferedReader(new InputStreamReader(
					((HttpURLConnection)(new URL("https://www.googleapis.com/oauth2/v3/tokeninfo?id_token=" + idToken.trim()))
					.openConnection()).getInputStream(), Charset.forName("UTF-8")));

			// Read information into a string buffer
			StringBuffer b = new StringBuffer();
			String lineStr = null;
			
			while ((lineStr = br.readLine()) != null) {
				b.append(lineStr + "\n");
			}

			// Transforming JSON string into Map<String, String>
			ObjectMapper objectMapper = new ObjectMapper();
			rtMap = objectMapper.readValue(b.toString(), objectMapper.getTypeFactory().constructMapType(Map.class, String.class, String.class));
		}
		catch (Exception e) {
			logger.error("Exception!", e);
		}
		finally {
			try {
				if (br != null) br.close();
			}
			catch (IOException e) {
				logger.error("Exception!", e);
			}
			
			if (rtMap == null) {
				logger.error("Fail to google authenticate from custom function!");
				return null;
			}
		}
		
		// At this time, 'rtMap' contains Google User data from google OAuth.
		// Now verify and check receive data.
		
		// 1) Verify email
		final String email_verified = (String)rtMap.get("email_verified");
		final String email = (String)rtMap.get("email");
		if (email_verified != null && email != null) {
			try {
				if (!Boolean.valueOf(email_verified) || !email.contains("@gmail.")) {
					logger.error("Email verification failed! - Required information missing in the token!");
					return null;
				}
			}
			catch (Exception e) {
				logger.error("Email verification failed with Exception!");
				return null;
			}
		}
		
		// 2) Check expiration time
		final String exp = (String)rtMap.get("exp");
		if (exp == null) {
			logger.error("Check expiration failed! - Required information missing in the token!");
			return null;
		}
		try {
			// The "exp" value is in seconds and Date().getTime is in millisecond
			if (Long.parseLong(exp + "000") > new java.util.Date().getTime()) {
				logger.error("Check expiration failed! - Token is expired!");
				return null;
			}
		}
		catch (Exception e) {
			logger.error("Check expiration failed with Exception!");
			return null;
		}
			
		// 3) Check audience
		final String aud = (String)rtMap.get("aud"); 
		final String azp = (String)rtMap.get("azp");
		if (aud != null && azp != null) {
			List<String> pom = Arrays.asList(_env.getProperty("auth.google.url"));

			if (!pom.contains(aud) || !pom.contains(azp)) {
				logger.error("Check audience failed! - Audiences differ!");
				return null;
			}
		}
		
		// Now verify and check is passed.
		// Return authenticated token's data.
		return rtMap;
	}
}
