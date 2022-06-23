package contact;

public class Contact {
	
	private String contactID;
	private String firstName;
	private String lastName;
	private String phoneNumber;
	private String address;

	public Contact(String contactID, String firstName, String lastName, String phoneNumber, String address) {
		
		if(contactID == null || contactID.length() > 10) {
			
			throw new IllegalArgumentException("Invalid contactID");
			}
		
		if(firstName == null || firstName.length() > 10) {
		
			throw new IllegalArgumentException("Invalid firstName");
		}
		
		if(lastName.length() > 10 || lastName == null) {
			
			throw new IllegalArgumentException("Invalid lastName");
		}
		
		if(phoneNumber.length() != 10 || phoneNumber == null) {
			
			throw new IllegalArgumentException("Invalid phoneNumber");
		}
		
		if(address.length()> 30 || address == null) {
			
			throw new IllegalArgumentException("Invalid address");
		}
	this. contactID = contactID;
	this.firstName = firstName;
	this.lastName = lastName;
	this.phoneNumber = phoneNumber;
	this.address = address;
}

	public String getContactID() {
		return contactID;
	}
	
	public void setContactId(String contactID) {
		if(contactID == null || contactID.length() > 10){
			throw new IllegalArgumentException("Invalid contactID");
	}
		this.contactID = contactID;
		}
		
	public String getFirstName() {
		return firstName;
	}
	
	public void setFirstName(String firstName) {
		if(firstName == null ||firstName.length() > 10) {
			throw new IllegalArgumentException("Invalid firstName");
	}
		
		this.firstName = firstName;
		}
	
	public String getLastName() {
		return lastName;
	}
	
	public void setlastName(String lastName) {
	if(lastName == null || lastName.length() > 10) {
		throw new IllegalArgumentException("Invalid lastName");
	}
	
		this.lastName = lastName;
		}
	public String getPhoneNumber() {
		return phoneNumber;
	}
	
	public void setphoneNumber(String phoneNumber) {
		if(phoneNumber == null || phoneNumber.length() != 10) {
			throw new IllegalArgumentException("Invalid phoneNumber");
		}
		
		this.phoneNumber = phoneNumber;
	}
	
	
	public String getAddress() {
		return address;
	}
	
	public void setaddress(String address) {
		if(address == null || address.length()> 30) {
		throw new IllegalArgumentException("Invalid address");
	}
	
		this.address = address;
}

}
