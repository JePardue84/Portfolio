package test;

import static org.junit.jupiter.api.Assertions.*;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import contact.Contact;



class ContactTest {


	@Test
	void testContact() {
		Contact contact = new Contact("01", "Joshua", "Pardue", "1234567890", "12 Pine");
		
		assertTrue(contact.getContactID().equals("01"));
		assertTrue(contact.getFirstName().equals("Joshua"));
		assertTrue(contact.getLastName().equals("Pardue"));
		assertTrue(contact.getPhoneNumber().equals("1234567890"));
		assertTrue(contact.getAddress().equals("12 Pine"));
		
	}
	
	
	@Test
	void testIDtoolong() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact("longtoolong", "Joshua", "Pardue", "1234567890", "12 Pine");
		});
		}
	
	@Test
	void testfirsttoolong() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact("01", "longtoolong", "Pardue", "1234567890", "12 Pine");
		});
		}
	
	@Test
	void testlasttoolong() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact("01", "Joshua", "longtoolong", "1234567890", "12 Pine");
		});
		}
	
	@Test
	void testphonetoolong() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact("01", "Joshua", "Pardue", "longtoolong", "12 Pine");
		});
		}
	
	
	@Test
	void testaddresstoolong() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact("01", "Joshua", "Pardue", "1234567890", "longtoolongaddressentirelytoolong000000000000");
		});
		}
	
	@Test
	void testIDnull() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact(null, "Joshua", "Pardue", "1234567890", "12 Pine");
		});
	}
	
	@Test
	void testfirstnull() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact("01", null, "Pardue", "1234567890", "12 Pine");
		});
	}
	
	@Test
	void testlastnull() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact("01", "Joshua", null, "1234567890", "12 Pine");
		});
	}
	
	@Test
	void testphonenull() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact("01", "Joshua", "Pardue", null, "12 Pine");
		});
	}
	
	@Test
	void testaddressnull() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact("01", "Joshua", "Pardue", "1234567890", null);
		});
	}
	
	
	@Test
	void testsetContactID() {
		Contact contact = new Contact("01", "Joshua", "Pardue", "1234567890", "12 Pine");
		String expectID = "01";
		contact.setContactId(expectID);
		String actualID = contact.getContactID();
		assertEquals(expectID, actualID);
		
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			contact.setContactId(null);
			
		});
		
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			contact.setContactId("0192093048230849238923");
			
		});
	}
	
	@Test
	void testsetFirstName() {
		Contact contact = new Contact("01", "Joshua", "Pardue", "1234567890", "12 Pine");
		String expectID = "Joshua";
		contact.setFirstName(expectID);
		String actualID = contact.getFirstName();
		assertEquals(expectID, actualID);
		
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			contact.setFirstName(null);
			
		});
		
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			contact.setFirstName("0192093048230849238923");
			
		});
	}
	
	@Test
	void testsetLastName() {
		Contact contact = new Contact("01", "Joshua", "Pardue", "1234567890", "12 Pine");
		String expectID = "Pardue";
		contact.setlastName(expectID);
		String actualID = contact.getLastName();
		assertEquals(expectID, actualID);
		
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			contact.setlastName(null);
			
		});
		
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			contact.setlastName("0192093048230849238923");
			
		});
	}
	
	@Test
	void testsetphoneNumberr() {
		Contact contact = new Contact("01", "Joshua", "Pardue", "1234567890", "12 Pine");
		String expectID = "1234567890";
		contact.setphoneNumber(expectID);
		String actualID = contact.getPhoneNumber();
		assertEquals(expectID, actualID);
		
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			contact.setphoneNumber(null);
			
		});
		
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			contact.setphoneNumber("0192093048230849238923");
			
		});
	}
	
	
	@Test
	void testsetaddress() {
		Contact contact = new Contact("01", "Joshua", "Pardue", "1234567890", "12 Pine");
		String expectID = "12 Pine";
		contact.setaddress(expectID);
		String actualID = contact.getAddress();
		assertEquals(expectID, actualID);
		
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			contact.setaddress(null);
			
		});
		
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			contact.setaddress("01920930482308492389230000000000000000000000000000000");
			
		});
	}
	
}