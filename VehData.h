// VehData.h
#ifndef VEH_DATA_H
#define VEH_DATA_H

class VehData
{
private:
    long initialEnergy;
    long currEnergy;
    long currShieldEnergy;
    long totEnergyUsed;
    long totShieldEnergyUsed;

public:
    VehData();
    VehData(long initialEnergy, long currEnergy, long currShieldEnergy, long totEnergyUsed, long totShieldEnergyUsed);

    long getInitialEnergy() const;
    void setInitialEnergy(long value);

    long getCurrentEnergy() const;
    void setCurrentEnergy(long value);

    long getCurrentShieldEnergy() const;
    void setCurrentShieldEnergy(long value);

    long getTotalEnergyUsed() const;
    void setTotalEnergyUsed(long value);

    long getTotalShieldEnergyUsed() const;
    void setTotalShieldEnergyUsed(long value);
};

#endif // VEH_DATA_H