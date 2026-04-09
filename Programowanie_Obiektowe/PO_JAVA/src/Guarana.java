public class Guarana extends Roslina
{
    public Guarana(int x, int y, Swiat swiat)
    {
        super(Stale.GUARANA_SILA, x, y, swiat);
    }

    @Override
    public void kolizja(Organizm atakujacy)
    {
        swiat.dodajLog(rysowanie() + " (" + x + ", " + y + ") zjedzony przez " + atakujacy.rysowanie() + " (" + atakujacy.getX() + ", " + atakujacy.getY() + ")");

        atakujacy.setSila(atakujacy.getSila() + Stale.GUARANA_BOOST_SILA);

        swiat.ustawOrganizmNa(atakujacy.getX(), atakujacy.getY(), null);
        atakujacy.setX(x);
        atakujacy.setY(y);
        swiat.ustawOrganizmNa(x, y, atakujacy);

        swiat.usunOrganizm(this);
    }

    @Override
    public char rysowanie()
    {
        return Stale.GUARANA;
    }

    @Override
    public String nazwa()
    {
        return Stale.GUARANA_S;
    }

    @Override
    public Organizm stworzPotomka(int x, int y)
    {
        return new Guarana(x, y, swiat);
    }
}