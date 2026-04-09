import java.io.*;
import java.util.*;

class User {
    String username;
    String password;

    User(String u, String p) {
        username = u;
        password = p;
    }
}

class Cloth {
    int id;
    String name;
    String size;
    String owner;

    Cloth(int id, String name, String size, String owner) {
        this.id = id;
        this.name = name;
        this.size = size;
        this.owner = owner;
    }
}

public class ClothesSwapApp {

    static final int MAX = 100;
    static ArrayList<Cloth> clothes = new ArrayList<>();

    // ===== LOAD =====
    static void loadClothes() {
        try (BufferedReader br = new BufferedReader(new FileReader("clothes.txt"))) {
            String line;
            while ((line = br.readLine()) != null) {
                String[] parts = line.split(",");
                clothes.add(new Cloth(
                        Integer.parseInt(parts[0]),
                        parts[1],
                        parts[2],
                        parts[3]
                ));
            }
        } catch (Exception e) {
            // ignore if file doesn't exist
        }
    }

    // ===== SAVE =====
    static void saveClothes() {
        try (PrintWriter pw = new PrintWriter(new FileWriter("clothes.txt"))) {
            for (Cloth c : clothes) {
                pw.println(c.id + "," + c.name + "," + c.size + "," + c.owner);
            }
        } catch (Exception e) {
            System.out.println("Error saving file");
        }
    }

    // ===== SIGNUP =====
    static void signup(Scanner sc) {
        try (PrintWriter pw = new PrintWriter(new FileWriter("users.txt", true))) {
            System.out.print("\nEnter new username: ");
            String user = sc.next();

            System.out.print("Enter new password: ");
            String pass = sc.next();

            pw.println(user + " " + pass);
            System.out.println("✅ Signup successful!");
        } catch (Exception e) {
            System.out.println("Error in signup");
        }
    }

    // ===== LOGIN =====
    static String login(Scanner sc) {
        System.out.print("\nUsername: ");
        String user = sc.next();

        System.out.print("Password: ");
        String pass = sc.next();

        try (Scanner file = new Scanner(new File("users.txt"))) {
            while (file.hasNext()) {
                String u = file.next();
                String p = file.next();

                if (u.equals(user) && p.equals(pass)) {
                    System.out.println("✅ Login successful!");
                    return user;
                }
            }
        } catch (Exception e) {
            System.out.println("No users found. Please signup first.");
        }

        System.out.println("❌ Invalid login!");
        return null;
    }

    // ===== ADD CLOTH =====
    static void addCloth(Scanner sc, String currentUser) {
        System.out.print("\nEnter Cloth ID: ");
        int id = sc.nextInt();
        sc.nextLine(); // consume newline

        System.out.print("Enter Cloth Name: ");
        String name = sc.nextLine();

        System.out.print("Enter Size: ");
        String size = sc.next();

        clothes.add(new Cloth(id, name, size, currentUser));
        saveClothes();

        System.out.println("✅ Cloth added successfully!");
    }

    // ===== VIEW =====
    static void viewClothes() {
        if (clothes.isEmpty()) {
            System.out.println("\nNo clothes available.");
            return;
        }

        System.out.println("\n------ AVAILABLE CLOTHES ------");
        for (Cloth c : clothes) {
            System.out.println("ID: " + c.id +
                    " | " + c.name +
                    " | Size: " + c.size +
                    " | Owner: " + c.owner);
        }
    }

    // ===== SEARCH =====
    static void searchBySize(Scanner sc) {
        System.out.print("\nEnter size: ");
        String size = sc.next();

        boolean found = false;

        for (Cloth c : clothes) {
            if (c.size.equals(size)) {
                System.out.println("ID: " + c.id +
                        " | " + c.name +
                        " | Owner: " + c.owner);
                found = true;
            }
        }

        if (!found) {
            System.out.println("No matching clothes.");
        }
    }

    // ===== SWAP =====
    static void requestSwap(Scanner sc) {
        System.out.print("\nEnter Cloth ID to swap: ");
        int id = sc.nextInt();

        for (Cloth c : clothes) {
            if (c.id == id) {
                System.out.println("🔄 Swap request sent to " + c.owner + "!");
                return;
            }
        }

        System.out.println("Cloth not found.");
    }

    // ===== MAIN =====
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        loadClothes();

        System.out.println("\n==== CLOTHES SWAP APP ====");
        System.out.println("1. Signup\n2. Login\nChoice: ");
        int authChoice = sc.nextInt();

        if (authChoice == 1) {
            signup(sc);
        }

        String currentUser = login(sc);
        if (currentUser == null) return;

        int choice;
        do {
            System.out.println("\n===== MENU =====");
            System.out.println("1. Add Cloth");
            System.out.println("2. View Clothes");
            System.out.println("3. Search by Size");
            System.out.println("4. Request Swap");
            System.out.println("5. Exit");
            System.out.print("Enter choice: ");

            choice = sc.nextInt();

            switch (choice) {
                case 1 -> addCloth(sc, currentUser);
                case 2 -> viewClothes();
                case 3 -> searchBySize(sc);
                case 4 -> requestSwap(sc);
            }

        } while (choice != 5);

        System.out.println("\n👋 Thank you for using Clothes Swap App!");
    }
}