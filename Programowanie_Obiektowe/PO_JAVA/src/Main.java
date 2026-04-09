import java.io.*;
import java.util.Scanner;

public class Main
{
    public static void main(String[] args)
    {
        Scanner scanner = new Scanner(System.in);
        System.out.println("Kacper Petla 198186");

        while (true)
        {
            System.out.print("\nWPISZ KOMENDE: ");
            String input = scanner.nextLine();
            if (input.isEmpty()) continue;

            char komenda = input.charAt(0);

            switch (komenda)
            {
                case Stale.NOWA_GRA:
                {
                    boolean running = true;
                    while (running)
                    {
                        System.out.print("WPISZ RODZAJ PLANSZY: ");
                        input = scanner.nextLine();
                        if (input.isEmpty()) continue;
                        komenda = input.charAt(0);

                        switch (komenda)
                        {
                            case Stale.KWADRAT:
                            {
                                System.out.print("WPISZ WYMIARY\n - szerokosc: ");
                                int szerokosc = scanner.nextInt();
                                System.out.print(" - wysokosc: ");
                                int wysokosc = scanner.nextInt();
                                scanner.nextLine();

                                Swiat swiat = new Swiat_Kwadrat(szerokosc, wysokosc);
                                swiat.zainicjujGre();
                                swiat.rozpocznijGre();
                                running = false;
                                break;
                            }
                            case Stale.HEX:
                            {
                                System.out.print("WPISZ WYMIARY\n - szerokosc: ");
                                int szerokosc = scanner.nextInt();
                                System.out.print(" - wysokosc: ");
                                int wysokosc = scanner.nextInt();
                                scanner.nextLine();

                                Swiat swiat = new Swiat_Hex(szerokosc, wysokosc);
                                swiat.zainicjujGre();
                                swiat.rozpocznijGre();
                                running = false;
                                break;
                            }
                            default:
                            {
                                System.out.println("NIEPOPRAWNA KOMENDA");
                                break;
                            }
                        }
                    }
                    break;
                }
                case Stale.WCZYTAJ:
                {
                    System.out.print("WPISZ PLIK Z ZAPISEM: ");
                    String nazwa_pliku = scanner.next() + ".txt";

                    try (BufferedReader reader = new BufferedReader(new FileReader(nazwa_pliku)))
                    {
                        String[] wymiary = reader.readLine().split(" ");
                        int x = Integer.parseInt(wymiary[1]);
                        int y = Integer.parseInt(wymiary[2]);

                        Swiat swiat = null;
                        if(wymiary[0].charAt(0) == Stale.HEX)
                        {
                            swiat = new Swiat_Hex(x, y);
                        }
                        else
                        {
                            swiat = new Swiat_Kwadrat(x, y);
                        }

                        swiat.wczytajGre(reader);
                        reader.close();

                        swiat.rozpocznijGre();
                    }
                    catch (Exception e)
                    {
                        System.out.println("NIE ZNALEZIONO TAKIEGO ZAPISU");
                    }
                    break;
                }
                case Stale.WYJDZ:
                {
                    System.out.println("Wyjście z gry.");
                    System.exit(0);
                    return;
                }
                default:
                {
                    System.out.println("NIEPOPRAWNA KOMENDA");
                    break;
                }
            }
        }
    }
}