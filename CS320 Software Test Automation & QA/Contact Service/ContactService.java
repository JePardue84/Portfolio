package contact;

import java.util.HashMap;


public class ContactService {
	
	private HashMap<String, Contact>map = new HashMap<String, Contact>();
	

	
	public void add(String contactID) {
		Contact person = map.get(contactID);
		if(person == null)  {
			person = new Contact(contactID, contactID, contactID, contactID, contactID);
		map.put(contactID, person);
		}
		
		else {
			throw new IllegalArgumentException("contact already exists");
		}
	}
	
	public void delete(String contactID) {
		map.remove(contactID);
	}
	
	public void updateFirstName(String contactID, String firstName) {
		Contact person = map.get(contactID);
		if(person == null) {
			throw new IllegalArgumentException("contact does not exist");
			
		}
		person.setFirstName(firstName);
	}
	
	public void updateLastName(String contactID, String lastName) {
		Contact person = map.get(contactID);
		if(person == null) {
			throw new IllegalArgumentException("contact does not exist");
		}
		person.setlastName(lastName);
	}
	
	public void updatephoneNumber(String contactID, String phoneNumber) {
		Contact person = map.get(contactID);
		if(person == null) {
			throw new IllegalArgumentException("contact does not exist");
	
		}
		person.setphoneNumber(phoneNumber);
	}
	
	public void updateaddress(String contactID, String address) {
		Contact person = map.get(contactID);
		if(person == null) {
			throw new IllegalArgumentException("contact does not exist");
		}
		person.setaddress(address);
	}
	
	public Contact getPerson(String contactID) {
		return this.map.get(contactID);
	}

}
