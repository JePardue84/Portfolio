package test;

import static org.junit.jupiter.api.Assertions.*;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import contact.Contact;
import contact.ContactService;
import task.Task;
import task.TaskService;




class ContactServiceTest {

	@Test
	void testaddContact() {
		ContactService person = new ContactService();
		person.add("01");
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			person.add("01");
			
		});
	}



	
	@Test
	void testdeleteContact() {
		ContactService person = new ContactService();
		person.add("01");
		person.delete("01");
		Contact contact = person.getPerson("01");
		assertNull(contact);
		
		
	}
	
	@Test
	void testupdateContact() {
		ContactService person = new ContactService();
		person.add("01");
		person.updateFirstName("01", "Joshua");
		Contact contact = person.getPerson("01");
		String expected = "Joshua";
		String actual = contact.getFirstName();
		assertEquals(expected,actual);
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			person.updateFirstName("01", "Joshua");
			
		});
		
	}
	
	
	@Test
	void testupdateTask() {
		TaskService service = new TaskService();
		service.add("01");
		service.updatetaskDescription("01", "special task");
		Task task = service.getService("01");
		String expected = "special task";
		String actual = task.gettaskDescription();
		assertEquals(expected,actual);
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			service.updatetaskDescription("03", "special task");
			
		});
		
	}

	
}


